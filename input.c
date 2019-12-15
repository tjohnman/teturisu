#include "input.h"

unsigned getActionIndexForKey(SDL_Keycode code) {
    switch(code) {
        default: return -1; break;
        case SDLK_LEFT: return left; break;
        case SDLK_RIGHT: return right; break;
        case SDLK_UP: return drop; break;
        case SDLK_DOWN: return down; break;
        case SDLK_z: return rotate_ccw; break;
        case SDLK_x: return rotate_cw; break;
        case SDLK_RETURN: return hold; break;
    }
}

void inputCheckEvent(SDL_Event * event) {
    unsigned action = getActionIndexForKey(event->key.keysym.sym);
    if(action == -1) return;

    if(event->type == SDL_KEYDOWN) {
        input.keys[action].pressed = SDL_TRUE;
        if(input.keys[action].repeat_lock) {
            input.keys[action].pressed_last_frame = SDL_FALSE;
        } else {
            input.keys[action].pressed_last_frame = SDL_TRUE;
            input.keys[action].keypress_time = SDL_GetTicks();
            input.keys[action].last_repeat_time = input.keys[action].keypress_time;
            input.keys[action].repeat_lock = SDL_TRUE;
        }
    } else if(event->type == SDL_KEYUP) {
        input.keys[action].pressed = SDL_FALSE;
        input.keys[action].pressed_last_frame = SDL_FALSE;
        input.keys[action].repeat_lock = SDL_FALSE;
        input.keys[action].key_code = SDLK_UNKNOWN;
    }
}

SDL_bool justPressed(SDL_Keycode code) {
    unsigned action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;
    
    SDL_bool result = input.keys[action].pressed_last_frame;
    input.keys[action].pressed_last_frame = SDL_FALSE;
    return result;
}

SDL_bool isKeyDown(SDL_Keycode code) {
    unsigned action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;

    return input.keys[action].pressed;
}

SDL_bool keyRepeat(SDL_Keycode code) {
    unsigned action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;

    unsigned time = SDL_GetTicks();
    SDL_bool result = input.keys[action].pressed && time > input.keys[action].keypress_time + 200 && time > input.keys[action].last_repeat_time + 25;
    if(result) input.keys[action].last_repeat_time = time;
    return result;
}
