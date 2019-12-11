#ifndef INCLUDES_H
#define INCLUDES_H

#define WIDTH 192
#define HEIGHT 144

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "helper.h"
#include "sprite.h"
#include "game_state.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * canvas;

#endif // INCLUDES_H