#include "texture.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>


b32 texture_load_from_file(Texture *t, char *path, SDL_Renderer *renderer)
{
    texture_free(t);

    SDL_Texture *new_texture = NULL;
    SDL_Surface *loaded_surface = IMG_Load(path);

    if (loaded_surface == NULL){
        SDL_WARN('Unable to load image!');
    }
    else
    {
        SDL_Surface *formatted_surface = SDL_ConvertSurfaceFormat( loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0 );
        if (formatted_surface == NULL) {
           SDL_WARN('Unable to convert loaded surface to display format');
        } else {
            printf("formatted surface:  %d, %d  \n", formatted_surface->w, formatted_surface->h);
            new_texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            formatted_surface->w,
                                            formatted_surface->h);
            if (new_texture == NULL) {
                SDL_WARN('Unable to create blank texture!');
            } else {
                SDL_SetTextureBlendMode( new_texture, SDL_BLENDMODE_BLEND );

                SDL_LockTexture(new_texture, NULL, &t->pixels, &t->pitch);
                memcpy(t->pixels, formatted_surface->pixels, formatted_surface->pitch*formatted_surface->h);

                t->width = formatted_surface->w;
                t->height = formatted_surface->h;

                u32 *pixels = (u32*)t->pixels;
                s32 pixel_count = (t->pitch / 4) * t->height;
                u32 color_key = SDL_MapRGB(formatted_surface->format, 0, 0xFF, 0xFF);
                u32 transparent = SDL_MapRGBA(formatted_surface->format, 0x00, 0xFF,0xFF, 0x00);
                s32 i = 0;
                for( i = 0; i < pixel_count; ++i ) {
					if( pixels[i] == color_key ){
						pixels[i] = transparent;
					}
				}

                SDL_UnlockTexture(new_texture);
                t->pixels = NULL;

            }
            SDL_FreeSurface(formatted_surface);
        }
        SDL_FreeSurface(loaded_surface);
    }
    t->SDLtex = new_texture;
    return t->SDLtex != NULL;
}

void texture_render_text(Texture *t, s32 x, s32 y,const char *text, r32 scale, SDL_Renderer* renderer)
{
    // this function assumes you load a 8x8 bitmap font texture
    // all glyphs alligned in ascii rows, google 'libtcod font image' when unsure.

    s32 count = strlen(text);
    s32 current_x = x;
    s32 current_y = y;
    s32 i = 0;
    for (i = 0; i < count; i+=1) {
        u8 ascii = (u8) text[i];
        if (ascii == 10) {
            current_x = x;
            current_y += 8*scale;
        } else {
            SDL_Rect src_rect = {(u8) (ascii % 16)*8, (u8) (ascii / 16)*8, 8, 8};
            SDL_Rect dest_rect = {current_x, current_y, 8*scale, 8*scale};

            SDL_RenderCopy(renderer, t->SDLtex, &src_rect, &dest_rect);
            current_x += 8*scale;
        }
    }
}


void texture_init(Texture *t)
{
    t->width = 0;
    t->height = 0;
    t->SDLtex = NULL;
}

void texture_free(Texture *t)
{
    if (t->SDLtex != NULL){
        SDL_DestroyTexture(t->SDLtex);
        t->SDLtex = NULL;
        t->width = 0;
        t->height = 0;
        t->pixels = NULL;
        t->pitch = 0;
    }
}

void texture_destroy(Texture *t)
{
    texture_free(t);
}

void texture_render_ex(Texture *t, s32 x, s32 y, SDL_Rect *clip, r32 angle, SDL_Point *center, SDL_RendererFlip flip, SDL_Renderer *renderer)
{
    SDL_Rect render_quad = {x, y, t->width, t->height};

    if (clip != NULL){
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopyEx( renderer, t->SDLtex, clip, &render_quad, angle, center, flip );
}

b32 texture_lock(Texture *t)
{
    b32 success = true;
    if (t->pixels != NULL) {
        printf("Texture is already locked!\n");
        success = false;
    } else {
        if (SDL_LockTexture(t->SDLtex, NULL, &t->pixels, &t->pitch) != 0) {
            printf("Unable to lock texture! %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;
}
b32 texture_unlock(Texture *t)
{
    b32 success = true;
    if (t->pixels == NULL) {
        printf("Texture is not locked!\n");
        success = false;
    } else {
        SDL_UnlockTexture(t->SDLtex);
        t->pixels = NULL;
        t->pitch = 0;
    }
    return success;
}

void texture_copy_pixels(Texture *t, void *pixels)
{
    if(t->pixels != NULL) {
        memcpy(t->pixels, pixels, t->pitch*t->height);
    }
}


u32 texture_get_pixel32(Texture *t, s32 x, s32 y)
{
    u32* pixels = (u32*)t->pixels;
    return pixels[( y * ( t->pitch / 4 ) ) + x ];
}

b32 texture_create_blank(Texture *t, s32 width, s32 height, SDL_TextureAccess access,  SDL_Renderer *renderer)
{
    t->SDLtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if(t->SDLtex == NULL) {
        SDL_WARN('Unable to create blank texture');
    } else {
        t->width = width;
        t->height = height;
    }
    return (t->SDLtex != NULL);
}
