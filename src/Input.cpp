#include "Input.h"

bool Input::is_key_down(SDL_Scancode key) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[key];
}
