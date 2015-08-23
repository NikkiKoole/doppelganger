#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

#include "defines.h"

typedef struct Texture
{
    SDL_Texture *SDLtex;
    s32 width;
    s32 height;
    s32 pitch;
    void *pixels;
} Texture;

void texture_init(Texture *t);
void texture_destroy(Texture *t);

b32 texture_load_from_file(Texture *t, char *path, SDL_Renderer *renderer);
void texture_render_text(Texture *t, s32 x, s32 y, const char *text, r32 scale, SDL_Renderer *renderer);

void texture_free(Texture *t);
void texture_render_ex(Texture *t, s32 x, s32 y, SDL_Rect *clip, r32 angle, SDL_Point *center, SDL_RendererFlip flip, SDL_Renderer *renderer);

b32 texture_lock(Texture *t);
b32 texture_unlock(Texture *t);

void texture_copy_pixels(Texture *t, void *pixels);
u32 texture_get_pixel32(Texture *t, s32 x, s32 y);

b32 texture_create_blank(Texture *t, s32 width, s32 height, SDL_TextureAccess access,  SDL_Renderer *renderer);

#endif
