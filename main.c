#include "includes.h"

int main(int argc, char * argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_CreateWindowAndRenderer(WIDTH * 2, HEIGHT * 2, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");

    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

    uint8_t done = SDL_FALSE;
    SDL_Event e;

    while(!done) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                done = SDL_TRUE;
                break;
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, canvas, 0, 0);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(canvas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}