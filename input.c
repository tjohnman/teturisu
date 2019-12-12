#include "input.h"

void inputCheckEvent(SDL_Event * event) {
    unsigned idx = -1;
    for(unsigned i=0; i<6; ++i) {
        if(input.pressed_keys[i].key_code == event->key.keysym.sym) {
            idx = i;
            break;
        }
    }

    if(idx == -1) {
        idx = (idx + input.pressed_keys_idx) % 6;
    } 

    if(event->type == SDL_KEYDOWN) {
        if(input.pressed_keys[idx].repeat_lock) {
            input.pressed_keys[idx].pressed_last_frame = SDL_FALSE;
        } else {
            input.pressed_keys[idx].pressed_last_frame = SDL_TRUE;
            input.pressed_keys[idx].keypress_time = SDL_GetTicks();
            input.pressed_keys[idx].last_repeat_time = input.pressed_keys[idx].keypress_time;
            input.pressed_keys[idx].repeat_lock = SDL_TRUE;
        }
        input.pressed_keys[idx].key_code = event->key.keysym.sym;
    } else if(event->type == SDL_KEYUP) {
        input.pressed_keys[idx].pressed_last_frame = SDL_FALSE;
        input.pressed_keys[idx].repeat_lock = SDL_FALSE;
        input.pressed_keys[idx].key_code = SDLK_UNKNOWN;
    }
}

SDL_bool justPressed(SDL_Keycode code) {
    unsigned idx = -1;
    for(unsigned i=0; i<6; ++i) {
        if(input.pressed_keys[i].key_code == code) {
            idx = i;
            break;
        }
    }

    if(idx == -1) return SDL_FALSE;
    
    SDL_bool result = input.pressed_keys[idx].key_code == code && input.pressed_keys[idx].pressed_last_frame;
    input.pressed_keys[idx].pressed_last_frame = SDL_FALSE;
    return result;
}

SDL_bool isKeyDown(SDL_Keycode code) {
    unsigned idx = -1;
    for(unsigned i=0; i<6; ++i) {
        if(input.pressed_keys[i].key_code == code) {
            idx = i;
            break;
        }
    }

    if(idx == -1) return SDL_FALSE;

    return input.pressed_keys[idx].key_code == code;
}

SDL_bool keyRepeat(SDL_Keycode code) {
    unsigned idx = -1;
    for(unsigned i=0; i<6; ++i) {
        if(input.pressed_keys[i].key_code == code) {
            idx = i;
            break;
        }
    }

    if(idx == -1) return SDL_FALSE;

    unsigned time = SDL_GetTicks();
    SDL_bool result = time > input.pressed_keys[idx].keypress_time + 300 && time > input.pressed_keys[idx].last_repeat_time + 50;
    if(result) input.pressed_keys[idx].last_repeat_time = time;
    return result;
}
