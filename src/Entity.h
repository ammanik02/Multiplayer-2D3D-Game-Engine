#include <SDL.h>
#include <string>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Entity : public std::enable_shared_from_this<Entity> {
public:
    size_t id;
    float x, y, z; // Added Z
    float vx, vy;
    float rx, ry, rz; // Rotation
    int w, h;
    Uint8 r, g, b, a;

    Entity(size_t id, float x, float y, float z, int w, int h, Uint8 r, Uint8 g, Uint8 b);
    virtual ~Entity() = default;

    void update(float dt) override;
    void render(SDL_Renderer* renderer);
    
    // Scripting
    void attach_script(const std::string& moduleName, const std::string& className);
    py::object script_instance;
    void move(float dx, float dy);
    
    SDL_Rect get_bounds() const;
};
