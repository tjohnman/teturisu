#include "includes.h"

void draw(SDL_Texture * texture, SDL_Rect * src, unsigned x, unsigned y, unsigned w, unsigned h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w == 0 ? src->w : w;
    dst.h = h == 0 ? src->h : h;

    SDL_SetRenderTarget(renderer, canvas);
    SDL_RenderCopy(renderer, texture, src, &dst);
    SDL_SetRenderTarget(renderer, 0);
}

void changeState(gameState_e state) {
    switch(currentState) {
        default:
        case menu:
            
        break;

        case game:
            gameStateWillChangeState(state);
            currentState = state;
        break;
    }
}