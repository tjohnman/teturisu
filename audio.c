#include "audio.h"

void playSound(Mix_Chunk * chunk) {
    if (!audioMuted) {
        Mix_PlayChannel(-1, chunk, 0);
    }
}