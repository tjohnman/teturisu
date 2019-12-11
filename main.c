#include "includes.h"
#include "helper.h"
#include "game_state.h"

void mainUpdate(double delta);
void mainDraw();

int main(int argc, char * argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_CreateWindowAndRenderer(WIDTH * SCALE, HEIGHT * SCALE, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");

    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

    uint8_t done = SDL_FALSE;
    SDL_Event e;
    uint32_t lastMillis, currentMillis;
    double delta = 0;

    init();
    changeState(game);

    while(!done) {
        currentMillis = SDL_GetTicks();
        delta = (currentMillis - lastMillis) / 1000.0;

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                done = SDL_TRUE;
                break;
            }
        }

        mainUpdate(delta);

        SDL_SetRenderTarget(renderer, canvas);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, 0);
        mainDraw();

        SDL_SetRenderTarget(renderer, 0);
        SDL_RenderCopy(renderer, canvas, 0, 0);
        SDL_RenderPresent(renderer);

        lastMillis = currentMillis;
    }

    cleanup();

    SDL_DestroyTexture(canvas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void mainUpdate(double delta) {
    switch(currentState) {
        default:
        case menu:
            
        break;

        case game:
            gameStateUpdate(delta);
        break;
    }
}

void mainDraw() {
    switch(currentState) {
        default:
        case menu:
            
        break;

        case game:
            gameStateDraw();
        break;
    }
}