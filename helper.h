#ifndef HELPER_H
#define HELPER_H

#include "sprite.h"

typedef enum {
    menu,
    game
} gameState_e;

gameState_e currentState;

struct {
    sprite_t * digitsSprite;
    sprite_t * blocksSprite;
    char * buffer;
} helperData;

void init();
void draw(SDL_Texture * texture, SDL_Rect * src, unsigned x, unsigned y, unsigned w, unsigned h);
void drawScaled(SDL_Texture * texture, SDL_Rect * src, unsigned x, unsigned y, unsigned w, unsigned h, double scale);
void drawNumber(unsigned number, unsigned x, unsigned y, uint8_t alignRight);
void changeState(gameState_e state);
void fillRect(unsigned x, unsigned y, unsigned w, unsigned h);
void cleanup();

#endif // HELPER_H
