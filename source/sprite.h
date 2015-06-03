#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "animation.h"


typedef struct Sprite
{
    Texture* texture;
    SDL_Rect clip;
    int32 frame_width;
    int32 frame_height;
    int32 current_frame;
    int32 elapsed_time;
} Sprite;

void sprite_init(Sprite* spr, Texture* t, SDL_Rect clip,  int32 frame_width, int32 frame_height);
void sprite_play_animation(Sprite* spr, Animation* anim);
#endif
