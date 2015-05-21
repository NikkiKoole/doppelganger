#include "texture.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

bool texture_load_from_file(texture* t, char* path, SDL_Renderer* renderer)
{
    texture_free(t);

    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path);
    if (loaded_surface == NULL){
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
    }
    else
    {
        SDL_SetColorKey(loaded_surface,
                        SDL_TRUE,
                        SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (new_texture == NULL){
            printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
        } else {
            t->width = loaded_surface->w;
            t->height = loaded_surface->h;
        }
        SDL_FreeSurface(loaded_surface);
    }
    t->tex = new_texture;
    return t->tex != NULL;
}


void init_texture(texture* t)
{
    t->width = 0;
    t->height = 0;
    t->tex = NULL;
}

void texture_free(texture* t)
{
    if (t->tex != NULL){
        SDL_DestroyTexture(t->tex);
        t->width = 0;
        t->height = 0;
    }
}

void texture_destroy(texture* t)
{
    texture_free(t);
}

void texture_set_color(texture* t, uint8 Red, uint8 Green, uint8 Blue)
{
    SDL_SetTextureColorMod(t->tex, Red, Green, Blue );
}

void texture_set_blend_mode(texture* t, SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(t->tex, mode);
}
void texture_set_alpha(texture* t, uint8 alpha)
{
    SDL_SetTextureAlphaMod(t->tex, alpha);
}

void texture_render(texture* t, int x,int y, SDL_Renderer* renderer)
{
    SDL_Rect render_quad = {x,y, t->width, t->height};
    SDL_RenderCopy(renderer, t->tex, NULL, &render_quad);
}
