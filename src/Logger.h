#pragma once
#include <iostream>
#include <string>

class Log {
public:
    template<typename T>
    static void Info(const T& msg) {
        std::cout << "[INFO] " << msg << std::endl;
    }

    template<typename T>
    static void Error(const T& msg) {
        std::cerr << "[ERROR] " << msg << std::endl;
    }
    
    // Simple variadic for multiple args could be added, but keeping it simple for now
    // or just overloading "<<" if we returned a stream wrapper.
    // For this portfolio: simplicity.
};
