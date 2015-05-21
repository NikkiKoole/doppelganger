#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct texture
{
    SDL_Texture* tex;
    int width;
    int height;
} texture;

void init_texture(texture* t);
void texture_destroy(texture* t);

bool texture_load_from_file(texture* t, char* path, SDL_Renderer* renderer);
void texture_free(texture* t);
void texture_render(texture* t, int x,int y, SDL_Renderer* renderer);

#endif
