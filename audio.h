#ifndef AUDIO_H
#define AUDIO_H

#include "includes.h"

extern SDL_bool audioMuted;
void playSound(Mix_Chunk * chunk);

#endif