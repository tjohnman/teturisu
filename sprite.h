#include "includes.h"

#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
    char * image_path;
    unsigned frame_width, frame_height;
    unsigned width, height;
    double current_frame;
    unsigned num_frames;
    unsigned x, y;
    SDL_Texture * texture;
} sprite_t;

sprite_t * spriteCreate(const char * path, unsigned frame_width, unsigned frame_height);
void spriteSetFrame(sprite_t * sprite, unsigned frame);
void spriteDraw(sprite_t * sprite);
void spriteDrawScaled(sprite_t * sprite, double scale);
void spriteDestroy(sprite_t * sprite);

#endif // SPRITE_H