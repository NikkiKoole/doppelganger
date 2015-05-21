#include "texture.h"
//#include <SDL2/SDL.h>
//#include <stdbool.h>
#include <SDL2/SDL_image.h>

bool texture_load_from_file(texture t, char* path, SDL_Renderer* renderer)
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
            printf("Unable to create texture from %s! SDL_Error:", path, SDL_GetError());
        } else {
            t.width = loaded_surface->w;
            t.height = loaded_surface->h;
        }
        SDL_FreeSurface(loaded_surface);
    }
    t.tex = new_texture;
    return t.tex != NULL;
}

texture texture_create()
{
    texture result = {
        .width = 0,
        .height = 0,
        .tex = NULL
    };
    return result;
}

void texture_free(texture t)
{
    if (t.tex != NULL){
        SDL_DestroyTexture(t.tex);
        t.width = 0;
        t.height = 0;
    }
}

void texture_destroy(texture t)
{
    texture_free(t);
}

void texture_render(texture t, int x,int y, SDL_Renderer* renderer)
{
    SDL_Rect render_quad = {x,y, t.width, t.height};
    SDL_RenderCopy(renderer, t.tex, NULL, &render_quad);
}
