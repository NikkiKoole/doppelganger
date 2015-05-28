#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "defines.h"

typedef struct Texture
{
    SDL_Texture* tex;
    int width;
    int height;
    int pitch;
    void* pixels;
} Texture;

void texture_init(Texture* t);
void texture_destroy(Texture* t);

bool texture_load_from_file(Texture* t, char* path, SDL_Renderer* renderer);

#ifdef _SDL_TTF_H
bool texture_load_from_rendered_text(Texture* t,char* text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer);
#endif

void texture_free(Texture* t);
void texture_render(Texture* t, int x,int y, SDL_Renderer* renderer);
void texture_render_ex(Texture* t, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer);

void texture_set_color(Texture* t, uint8 Red, uint8 Green, uint8 Blue);
void texture_set_blend_mode(Texture* t, SDL_BlendMode mode);
void texture_set_alpha(Texture* t, uint8 alpha);

bool texture_lock(Texture* t);
bool texture_unlock(Texture* t);

void texture_copy_pixels(Texture* t, void* pixels);
uint32 texture_get_pixel_32(Texture* t, uint16 x, uint16 y);

bool texture_create_blank(Texture* t, int width, int height, SDL_TextureAccess access,  SDL_Renderer* renderer);
#endif
