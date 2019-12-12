#include "includes.h"

#ifndef INPUT_H
#define INPUT_H

typedef struct {
    SDL_Keycode key_code;
    uint32_t keypress_time, last_repeat_time;
    SDL_bool repeat_lock, pressed_last_frame;
} key_action_t;

struct {
    key_action_t pressed_keys[6];
    unsigned pressed_keys_idx;
} input;

void inputCheckEvent(SDL_Event * event);
SDL_bool justPressed(SDL_Keycode code);
SDL_bool isKeyDown(SDL_Keycode code);
SDL_bool keyRepeat(SDL_Keycode code);

#endif // INPUT_H