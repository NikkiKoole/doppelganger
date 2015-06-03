#include "texture.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

#define SDL_WARN(msg) printf("%s SDL Error: %s \t\t(%s, %s:%d)\n", #msg, SDL_GetError(), __FILE__, __FUNCTION__, __LINE__);


bool texture_load_from_file(Texture* t, char* path, SDL_Renderer* renderer)
{
    texture_free(t);

    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path);

    if (loaded_surface == NULL){
        SDL_WARN('Unable to load image!');
    }
    else
    {
        SDL_Surface* formatted_surface = SDL_ConvertSurfaceFormat( loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0 );
        if (formatted_surface == NULL) {
           SDL_WARN('Unable to convert loaded surface to display format');
        } else {
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

                uint32* pixels = (uint32*)t->pixels;
                int32 pixel_count = (t->pitch / 4) * t->height;
                uint32 color_key = SDL_MapRGB(formatted_surface->format, 0, 0xFF, 0xFF);
                uint32 transparent = SDL_MapRGBA(formatted_surface->format, 0x00, 0xFF,0xFF, 0x00);
                int32 i = 0;
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
    t->tex = new_texture;
    return t->tex != NULL;
}

void texture_render_text(Texture* t, int32 x, int32 y,const char* text, real32 scale, SDL_Renderer* renderer)
{
    // this function assumes you load a 8x8 bitmap font texture
    // all glyphs alligned in ascii rows, google 'libtcod font image' when unsure.

    int32 count = strlen(text);
    int32 current_x = x;
    int32 current_y = y;
    int32 i = 0;
    for (i = 0; i < count; i+=1) {
        uint8 ascii = (uint8) text[i];
        if (ascii == 10) {
            current_x = x;
            current_y += 8*scale;
        } else {
            SDL_Rect src_rect = {(uint8) (ascii % 16)*8, (uint8) (ascii / 16)*8, 8, 8};
            SDL_Rect dest_rect = {current_x, current_y, 8*scale, 8*scale};

            SDL_RenderCopy(renderer, t->tex, &src_rect, &dest_rect);
            current_x += 8*scale;
        }
    }
}


void texture_init(Texture* t)
{
    t->width = 0;
    t->height = 0;
    t->tex = NULL;
}

void texture_free(Texture* t)
{
    if (t->tex != NULL){
        SDL_DestroyTexture(t->tex);
        t->tex = NULL;
        t->width = 0;
        t->height = 0;
        t->pixels = NULL;
        t->pitch = 0;
    }
}

void texture_destroy(Texture* t)
{
    texture_free(t);
}

void texture_set_color(Texture* t, uint8 Red, uint8 Green, uint8 Blue)
{
    SDL_SetTextureColorMod(t->tex, Red, Green, Blue );
}

void texture_set_blend_mode(Texture* t, SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(t->tex, mode);
}
void texture_set_alpha(Texture* t, uint8 alpha)
{
    SDL_SetTextureAlphaMod(t->tex, alpha);
}





void texture_render(Texture* t, int32 x,int32 y, SDL_Renderer* renderer)
{
    SDL_Rect render_quad = {x,y, t->width, t->height};
    SDL_RenderCopy(renderer, t->tex, NULL, &render_quad);
}

void texture_render_ex(Texture* t, int32 x, int32 y, SDL_Rect* clip, real32 angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
    SDL_Rect render_quad = {x, y, t->width, t->height};

    if (clip != NULL){
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopyEx( renderer, t->tex, clip, &render_quad, angle, center, flip );
}

bool texture_lock(Texture* t)
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
bool texture_unlock(Texture* t)
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

void texture_copy_pixels(Texture* t, void* pixels)
{
    if(t->pixels != NULL) {
        memcpy(t->pixels, pixels, t->pitch*t->height);
    }
}


uint32 texture_get_pixel32(Texture* t, int32 x, int32 y)
{
    uint32* pixels = (uint32*)t->pixels;
    return pixels[( y * ( t->pitch / 4 ) ) + x ];
}

bool texture_create_blank(Texture* t, int32 width, int32 height, SDL_TextureAccess access,  SDL_Renderer* renderer)
{
    t->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if(t->tex == NULL) {
        SDL_WARN('Unable to create blank texture');
    } else {
        t->width = width;
        t->height = height;
    }
    return (t->tex != NULL);
}

void texture_set_as_rendertarget(Texture* t, SDL_Renderer* renderer)
{
	SDL_SetRenderTarget( renderer, t->tex );
}
