#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

class Scripting {
public:
    static void init();
};
