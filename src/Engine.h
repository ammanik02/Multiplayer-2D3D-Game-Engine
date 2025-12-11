#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include <vector>
#include "Entity.h"
#include "Network.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Engine {
public:
    Engine();
    ~Engine();

    // Init: if is_server is true, we run headless (no window)
    bool init(const json& config, bool is_server = false);
    void run();
    void add_entity(std::shared_ptr<Entity> entity);
    
    // Access to network
    NetworkManager& get_network() { return network; }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool is_running = false;
    bool is_server_mode = false;
    
    NetworkManager network;
    std::vector<std::shared_ptr<Entity>> entities;

    void update(float dt);
    void render();
};
