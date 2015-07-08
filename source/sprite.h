#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "animation.h"

typedef struct Sprite
{
    Texture *texture;
    SDL_Rect clip;
    int32 frame_width;
    int32 frame_height;
    int32 current_frame;
    int32 elapsed_time;
} Sprite;

void sprite_init(Sprite *spr, Texture *t, SDL_Rect clip,  int32 frame_width, int32 frame_height);
void sprite_update_elapsed_time(Sprite *spr, int32 duration);
SDL_Rect sprite_get_current_frame(Sprite *spr, Animation *anim);
#endif
