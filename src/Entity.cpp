#include "Entity.h"

Entity::Entity(size_t id, float x, float y, float z, int w, int h, Uint8 r, Uint8 g, Uint8 b)
    : id(id), x(x), y(y), z(z), w(w), h(h), r(r), g(g), b(b), a(255), vx(0), vy(0), rx(0), ry(0), rz(0) {}

void Entity::update(float dt) {
    if (script_instance) {
        try {
            script_instance.attr("update")(dt);
        } catch (const std::exception& e) {
            // printf("Script error: %s\n", e.what()); // Uncomment for debug
        }
    }
    
    x += vx * dt;
    y += vy * dt;
    // Simple rotation integration for demo
    // rx += dt; 
    ry += dt; // Spin on Y
}

void Entity::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect = { (int)x, (int)y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void Entity::move(float dx, float dy) {
    x += dx;
    y += dy;
}

    return { (int)x, (int)y, w, h };
}

void Entity::attach_script(const std::string& moduleName, const std::string& className) {
    try {
        py::module_ module = py::module_::import(moduleName.c_str());
        py::object class_obj = module.attr(className.c_str());
        script_instance = class_obj(shared_from_this());
    } catch (const std::exception& e) {
        printf("Failed to attach script %s.%s: %s\n", moduleName.c_str(), className.c_str(), e.what());
    }
}
