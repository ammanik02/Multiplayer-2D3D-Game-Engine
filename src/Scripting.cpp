#include "Scripting.h"
#include "Entity.h"
#include "Input.h"

// Define the embedded module
PYBIND11_EMBEDDED_MODULE(engine, m) {
    // Expose Input
    py::class_<Input>(m, "Input")
        .def_static("is_key_down", &Input::is_key_down);

    // Expose Entity
    py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity")
        .def_readwrite("x", &Entity::x)
        .def_readwrite("y", &Entity::y)
        .def_readwrite("z", &Entity::z)
        .def_readwrite("vx", &Entity::vx)
        .def_readwrite("vy", &Entity::vy)
        .def_readwrite("ry", &Entity::ry)
        .def("move", &Entity::move);
}

void Scripting::init() {
    // We don't need to do much here if we use scoped_interpreter in main or Engine
    // But we might want to set up path
    try {
        py::module_::import("sys").attr("path").attr("append")("./scripts");
    } catch (const std::exception& e) {
        printf("Error initializing scripting: %s\n", e.what());
    }
}
