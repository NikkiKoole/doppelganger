#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "animation.h"

typedef struct Sprite
{
    Texture* texture;
    SDL_Rect clip;
    int frame_width;
    int frame_height;
    int current_frame;
    int elapsed_time;
} Sprite;

void sprite_init(Sprite* spr, Texture* t, SDL_Rect clip,  int frame_width, int frame_height);
void sprite_play_animation(Sprite* spr, Animation* anim);
#endif
