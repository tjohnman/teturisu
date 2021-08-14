#include "includes.h"
#include "helper.h"
#include "game_state.h"
#include "input.h"

SDL_Window * window;
SDL_Renderer * renderer;
gameState_e currentState;
helperData_t helperData;
gameState_t gameState;

void mainUpdate(double delta);
void mainDraw();

int main(int argc, char * argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_OGG);

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(16);

    window = SDL_CreateWindow("Teturisu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*SCALE, HEIGHT*SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");

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
            } else {
                inputCheckEvent(&e);
            }
        }

        mainUpdate(delta);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, 0);

        mainDraw();

        SDL_RenderPresent(renderer);

        lastMillis = currentMillis;
    }

    cleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    Mix_Quit();

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