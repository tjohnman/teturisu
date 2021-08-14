#include "includes.h"

#ifndef INPUT_H
#define INPUT_H

extern enum {
    left, right, drop, down, rotate_cw, rotate_ccw, hold, pause
} input_action;

typedef struct {
    SDL_Keycode key_code;
    uint32_t keypress_time, last_repeat_time;
    SDL_bool repeat_lock, pressed_last_frame, pressed;
} key_action_t;

typedef struct {
    key_action_t keys[8];
} input_t;

extern input_t input;

unsigned getActionIndexForKey(SDL_Keycode code);
void inputCheckEvent(SDL_Event * event);
SDL_bool justPressed(SDL_Keycode code);
SDL_bool isKeyDown(SDL_Keycode code);
SDL_bool keyRepeat(SDL_Keycode code);

#endif // INPUT_H