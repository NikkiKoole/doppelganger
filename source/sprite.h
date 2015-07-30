#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "animation.h"

typedef struct Sprite
{
    Texture *texture;
    SDL_Rect clip;
    s32 frame_width;
    s32 frame_height;
    s32 current_frame;
    s32 elapsed_time;
} Sprite;

void sprite_init(Sprite *spr, Texture *t, SDL_Rect clip,  s32 frame_width, s32 frame_height);
void sprite_update_elapsed_time(Sprite *spr, s32 duration);
SDL_Rect sprite_get_current_frame(Sprite *spr, Animation *anim);
#endif
