#include "input.h"

input_t input;

unsigned getActionIndexForKey(SDL_Keycode code) {
    switch(code) {
        default: return -1; break;
        case SDLK_LEFT: return left; break;
        case SDLK_RIGHT: return right; break;
        case SDLK_DOWN: return down; break;
        case SDLK_SPACE: return drop; break;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
        case SDLK_z: return rotate_ccw; break;
        case SDLK_UP:
        case SDLK_x: return rotate_cw; break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
        case SDLK_c: return hold; break;
        case SDLK_F1: case SDLK_ESCAPE: return pause; break;
        case SDLK_m: return mute; break;
    }
}

void inputCheckEvent(SDL_Event * event) {
    signed action = getActionIndexForKey(event->key.keysym.sym);
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
    signed action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;
    
    SDL_bool result = input.keys[action].pressed_last_frame;
    input.keys[action].pressed_last_frame = SDL_FALSE;
    return result;
}

SDL_bool isKeyDown(SDL_Keycode code) {
    signed action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;

    return input.keys[action].pressed;
}

SDL_bool keyRepeat(SDL_Keycode code) {
    signed action = getActionIndexForKey(code);
    if(action == -1) return SDL_FALSE;

    unsigned time = SDL_GetTicks();
    SDL_bool result = input.keys[action].pressed && time > input.keys[action].keypress_time + 200 && time > input.keys[action].last_repeat_time + 25;
    if(result) input.keys[action].last_repeat_time = time;
    return result;
}
