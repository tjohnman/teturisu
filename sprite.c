#include "sprite.h"
#include "helper.h"

sprite_t * spriteCreate(const char * path, unsigned frame_width, unsigned frame_height) {
    sprite_t * sprite = (sprite_t *) malloc(sizeof(sprite_t));

    sprite->image_path = (char *) malloc(strlen(path));
    strncpy(sprite->image_path, path, strnlen(path, 256));

    sprite->current_frame = 0;
    sprite->frame_width = frame_width;
    sprite->frame_height = frame_height;

    SDL_Surface * surface = IMG_Load(path);
    sprite->width = surface->w;
    sprite->height = surface->h;
    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    sprite->x = 0;
    sprite->y = 0;
    sprite->num_frames = sprite->width / sprite->frame_width;

    return sprite;
}

void spriteSetFrame(sprite_t * sprite, unsigned frame) {
    sprite->current_frame = (unsigned) (frame % sprite->num_frames);
}

void spriteDraw(sprite_t * sprite) {
    SDL_Rect src;
    src.x = sprite->current_frame * sprite->frame_width;
    src.y = 0;
    src.w = sprite->frame_width;
    src.h = sprite->frame_height;

    draw(sprite->texture, &src, sprite->x, sprite->y, 0, 0);
}

void spriteDestroy(sprite_t * sprite) {
    free(sprite->image_path);
    SDL_DestroyTexture(sprite->texture);
    free(sprite);
}
