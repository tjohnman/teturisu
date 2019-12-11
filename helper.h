#ifndef HELPER_H
#define HELPER_H

typedef enum {
    menu,
    game
} gameState_e;

gameState_e currentState;

void draw(SDL_Texture * texture, SDL_Rect * src, unsigned x, unsigned y, unsigned w, unsigned h);
void changeState(gameState_e state);

#endif // HELPER_H