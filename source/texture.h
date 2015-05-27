#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "defines.h"

typedef struct texture
{
    SDL_Texture* tex;
    int width;
    int height;
    int pitch;
    void* pixels;
} texture;

void texture_init(texture* t);
void texture_destroy(texture* t);

bool texture_load_from_file(texture* t, char* path, SDL_Renderer* renderer, SDL_Window* window);
void texture_free(texture* t);
void texture_render(texture* t, int x,int y, SDL_Renderer* renderer);
void texture_render_ex(texture* t, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer);

void texture_set_color(texture* t, uint8 Red, uint8 Green, uint8 Blue);
void texture_set_blend_mode(texture* t, SDL_BlendMode mode);
void texture_set_alpha(texture* t, uint8 alpha);

bool texture_lock(texture* t);
bool texture_unlock(texture* t);

#endif
