#include "Engine.h"
#include "Scripting.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Engine::Engine() {}

Engine::~Engine() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Engine::init(const json& config, bool is_server) {
    // Init Python Scripting
    Scripting::init();
    
    is_server_mode = is_server;
    is_running = true;

    // Load settings
    std::string title = "Game Engine";
    int width = 800;
    int height = 600;
    int port = 1234;

    if (config.contains("window")) {
        auto& w = config["window"];
        if (w.contains("title")) title = w["title"];
        if (w.contains("width")) width = w["width"];
        if (w.contains("height")) height = w["height"];
    }
    if (config.contains("network")) {
        if (config["network"].contains("port")) port = config["network"]["port"];
    }

    if (is_server_mode) {
        std::cout << "Initializing HEADLESS Server on port " << port << "..." << std::endl;
        // Skip SDL Video init
        if (!network.init_server(port)) {
            return false;
        }
        return true;
    }

    // Client Mode
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // OpenGL Attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create Context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    // Default Shader
    // Basic shader for coloring
    std::string vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    std::string fShader = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 color;
        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";
    
    defaultShader = std::make_shared<Shader>(vShader, fShader);
    cubeMesh = std::make_shared<Mesh>();
    cubeMesh->load_cube();

    std::string host = "127.0.0.1";
    if (config.contains("network") && config["network"].contains("default_host")) {
        host = config["network"]["default_host"];
    }

    if (!network.init_client(host, port)) {
         std::cout << "Warning: Could not connect to server (running offline?)" << std::endl;
    }

    return true;
}

void Engine::add_entity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

void Engine::run() {
    Uint64 last_time = SDL_GetPerformanceCounter();

    while (is_running) {
        // Calculate Delta Time
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last_time) / (float)SDL_GetPerformanceFrequency();
        last_time = now;

        // Input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        // Update
        // Network Update
        network.update();
        
        // Server: Process Inputs
        if (is_server_mode) {
             auto inputs = network.pop_inputs();
             for (auto& inp : inputs) {
                 // Map client_id/peer to an Entity!
                 // For Phase 3, we hack it:
                 // If ID matches an Entity's 'owner_id'??
                 // Actually we haven't implemented ownership yet.
                 // Let's assume Entity 1 is Player 1 (Client with pointer X)
                 // This is tricky without a lobby.
                 // HACK: First input we see becomes Entity 1's controller.
                 // Let's just find Entity ID 1 and apply input to it.
                 if (!entities.empty()) {
                     // Assume Entity 1 is the player.
                     // In real app we map inputs.client_id to entity.id
                     // For now: Apply to first entity if it's a player
                     // We need to pass input to the entity so the script updates it.
                     // The script checks Engine::Input.
                     // We need to INJECT the input into a "Virtual Input" state for that entity?
                     // Or, we update the entity's velocity directly here based on input?
                     // Let's update velocity directly for the demo proof.
                     // A better way: Modify Input class to allow "Setting" state from network,
                     // but Input is static global. That's a problem for multiplayer server (handling multiple inputs).
                     // Solution: Entity has its own InputState? or we pass InputState to update().
                     
                     // Direct manipulation for Phase 3 Proof:
                     if (inp.left) entities[0]->vx = -200;
                     else if (inp.right) entities[0]->vx = 200;
                     else entities[0]->vx = 0;
                     
                     if (inp.up) entities[0]->vy = -200;
                     else if (inp.down) entities[0]->vy = 200;
                     else entities[0]->vy = 0;
                 }
             }
        }
        
        update(dt);
        
        // Server: Broadcast State
        if (is_server_mode) {
             std::vector<EntityState> states;
             for(auto& e : entities) {
                 states.push_back({e->id, e->x, e->y});
             }
             network.broadcast_state(states);
        }
        
        // Client: Apply States
        if (!is_server_mode) {
            for(auto& state : network.latest_states) {
                // Find entity
                for(auto& e : entities) {
                    if (e->id == state.id) {
                         // Lerp? Snap for now.
                         e->x = state.x;
                         e->y = state.y;
                    }
                }
            }
            
            // Send Input
            // Check Input::is_key_down and send
            bool l = Input::is_key_down(SDL_SCANCODE_LEFT) || Input::is_key_down(SDL_SCANCODE_A);
            bool r = Input::is_key_down(SDL_SCANCODE_RIGHT) || Input::is_key_down(SDL_SCANCODE_D);
            bool u = Input::is_key_down(SDL_SCANCODE_UP) || Input::is_key_down(SDL_SCANCODE_W);
            bool d = Input::is_key_down(SDL_SCANCODE_DOWN) || Input::is_key_down(SDL_SCANCODE_S);
            network.send_input(l, r, u, d);
        }

        // Render
        if (!is_server_mode) {
            render();
        } else {
             // Maybe sleep a bit to not hog CPU 
             SDL_Delay(16);
        }
    }
}

void Engine::update(float dt) {
    for (auto& entity : entities) {
        entity->update(dt);
    }
}

void Engine::render() {
    // OLDSDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark background
    // OLD SDL_RenderClear(renderer);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    defaultShader->use();
    
    // Transformations
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -600.0f)); // Camera back 600 units
    // Or closer if we normalize coordinates. Our coordinates are pixels (e.g. 400, 300).
    // Let's use an Ortho projection for pixel-perfect 2D-in-3D, or perspective looking at pixel plane?
    // Let's use Perspective.
    // If objects are at z=0, and we want to see x=0..800, y=0..600.
    // We can position camera at center (400, 300, 800) and look at (400, 300, 0).
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 2000.0f);
    view = glm::lookAt(glm::vec3(400, 300, 600), glm::vec3(400, 300, 0), glm::vec3(0, 1, 0));
    // Wait, coordinate system: Y down in SDL (0 top). GL Y up (0 bottom).
    // Let's invert Y in projection or view?
    // Easier: Just live with inverted Y for now or flip camera up vector.
    // Valid lookAt up is (0, -1, 0) to match SDL Y-down feel?
    view = glm::lookAt(glm::vec3(400, 300, 600), glm::vec3(400, 300, 0), glm::vec3(0, -1, 0)); // Flip Y ??
    // Let's try standard Y-up (0,-1,0) might make it look right if we configured 2D that way.
    // Actually SDL 0,0 is top-left.
    // If we look from +Z, Y-down means we need Up vector (0, 1, 0) but looking at something "down"?
    // Let's just stick to standard GL and mapping might be flipped. 
    // We can scale model -1 on Y.

    defaultShader->setMat4("projection", projection);
    defaultShader->setMat4("view", view);

    for (auto& entity : entities) {
        // Model Matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(entity->x, entity->y, entity->z));
        
        // Rotation
        model = glm::rotate(model, entity->ry, glm::vec3(0.0f, 1.0f, 0.0f)); // Spin Y
        
        // Scale (using w, h as size)
        model = glm::scale(model, glm::vec3(entity->w, entity->h, entity->w)); // Cube

        defaultShader->setMat4("model", model);
        defaultShader->setVec3("color", glm::vec3(entity->r/255.0f, entity->g/255.0f, entity->b/255.0f));
        
        cubeMesh->draw();
    }

    SDL_GL_SwapWindow(window);
}
