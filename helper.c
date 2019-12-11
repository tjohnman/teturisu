#include "includes.h"
#include "helper.h"
#include "game_state.h"

void init() {
    helperData.digitsSprite = spriteCreate("assets/digits.png", 8, 9);
    helperData.blocksSprite = spriteCreate("assets/blocks.png", 8, 8);
    helperData.buffer = (char *) malloc(128);
}

void draw(SDL_Texture * texture, SDL_Rect * src, unsigned x, unsigned y, unsigned w, unsigned h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w == 0 ? src->w : w;
    dst.h = h == 0 ? src->h : h;

    SDL_RenderCopy(renderer, texture, src, &dst);
}

void changeState(gameState_e state) {
    switch(currentState) {
        default:
        case menu:
            
        break;

        case game:
            gameStateWillChangeState(state);
        break;
    }

    switch(state) {
        default:
        case menu:
            
        break;

        case game:
            gameStateInitialize();
        break;
    }

    currentState = state;
}

void fillRect(unsigned x, unsigned y, unsigned w, unsigned h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void drawNumber(unsigned number, unsigned x, unsigned y, uint8_t alignRight) {
    SDL_itoa(number, helperData.buffer, 10);
    unsigned length = strlen(helperData.buffer);

    for(unsigned i = 0; i<length; ++i) {
        spriteSetFrame(helperData.digitsSprite, helperData.buffer[i] - 48);

        if(alignRight) {
            helperData.digitsSprite->x = x + helperData.digitsSprite->frame_width*i - length * helperData.digitsSprite->frame_width;
        } else {
            helperData.digitsSprite->x = x + helperData.digitsSprite->frame_width*i;
        }
        helperData.digitsSprite->y = y;
        spriteDraw(helperData.digitsSprite);
    }
}

void cleanup() {
    free(helperData.buffer);
    spriteDestroy(helperData.digitsSprite);
    spriteDestroy(helperData.blocksSprite);
}