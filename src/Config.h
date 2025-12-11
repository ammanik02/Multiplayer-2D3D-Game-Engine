#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <iostream>

using json = nlohmann::json;

class Config {
public:
    static json load(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) {
            std::cerr << "Failed to open config file: " << path << std::endl;
            return json::object();
        }
        return json::parse(f);
    }
};
