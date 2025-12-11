#pragma once
#include <SDL.h>

class Input {
public:
    // Check if a specific key is currently held down
    static bool is_key_down(SDL_Scancode key);
};
