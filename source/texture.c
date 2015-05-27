#include "texture.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>


bool texture_load_from_file(texture* t, char* path, SDL_Renderer* renderer, SDL_Window* window)
{
    texture_free(t);

    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path);
    
    if (loaded_surface == NULL){
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
    }
    else
    {
        SDL_Surface* formatted_surface = SDL_ConvertSurfaceFormat( loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0 );
        if (formatted_surface == NULL) {
           printf("Unable to convert loaded surface to display format!, SDL_Error: %s\n", SDL_GetError());
        } else {
            new_texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            formatted_surface->w,
                                            formatted_surface->h);
            if (new_texture == NULL) {
                printf("Unable to create blank texture error: %s\n", SDL_GetError());
            } else {
                SDL_SetTextureBlendMode( new_texture, SDL_BLENDMODE_BLEND );

                SDL_LockTexture(new_texture, NULL, &t->pixels, &t->pitch);
                memcpy(t->pixels, formatted_surface->pixels, formatted_surface->pitch*formatted_surface->h);
                SDL_UnlockTexture(new_texture);
                t->pixels = NULL;
                t->width = formatted_surface->w;
                t->height = formatted_surface->h;
            }
            SDL_FreeSurface(formatted_surface);
        }
        SDL_FreeSurface(loaded_surface);
    }
    t->tex = new_texture;
    return t->tex != NULL;
}


void texture_init(texture* t)
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

void texture_render_ex(texture* t, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
    SDL_Rect render_quad = {x, y, t->width, t->height};
    
    if (clip != NULL){
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopyEx( renderer, t->tex, clip, &render_quad, angle, center, flip );
}

bool texture_lock(texture* t)
{
    bool success = true;
    if (t->pixels != NULL) {
        printf("Texture is already locked!\n");
        success = false;
    } else {
        if (SDL_LockTexture(t->tex, NULL, &t->pixels, &t->pitch) != 0) {
            printf("Unable to lock texture! %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;
}
bool texture_unlock(texture* t) 
{
    bool success = true;
    if (t->pixels == NULL) {
        printf("Texture is not locked!\n");
        success = false;
    } else {
        SDL_UnlockTexture(t->tex);
        t->pixels = NULL;
        t->pitch = 0;
    }
    return success;
}





