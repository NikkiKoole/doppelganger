#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "defines.h"

typedef struct Texture
{
    SDL_Texture* tex;
    uint32 width;
    uint32 height;
    int32 pitch;
    void* pixels;
} Texture;

void texture_init(Texture* t);
void texture_destroy(Texture* t);

bool texture_load_from_file(Texture* t, char* path, SDL_Renderer* renderer);
void texture_render_text(Texture* t, int32 x, int32 y, const char* text, real32 scale, SDL_Renderer* renderer);

void texture_free(Texture* t);

void texture_render_part(Texture* t, SDL_Rect* source, SDL_Rect* dest, SDL_Renderer* renderer);
void texture_render(Texture* t, int32 x,int32 y, SDL_Renderer* renderer);
void texture_render_ex(Texture* t, int32 x, int32 y, SDL_Rect* clip, real32 angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer);

void texture_set_color(Texture* t, uint8 Red, uint8 Green, uint8 Blue);
void texture_set_blend_mode(Texture* t, SDL_BlendMode mode);
void texture_set_alpha(Texture* t, uint8 alpha);

bool texture_lock(Texture* t);
bool texture_unlock(Texture* t);

void texture_copy_pixels(Texture* t, void* pixels);
uint32 texture_get_pixel32(Texture* t, int32 x, int32 y);

bool texture_create_blank(Texture* t, int32 width, int32 height, SDL_TextureAccess access,  SDL_Renderer* renderer);
void texture_set_as_rendertarget(Texture* t, SDL_Renderer* renderer);

#endif
