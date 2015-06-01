#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"

typedef struct Sprite
{
    Texture* texture;
    SDL_Rect clip;
    int frame_width;
    int frame_height;
    int current_frame;
    int elapsed_time;

} Sprite;

bool prepare_sprite(Sprite* spr, Texture* t, SDL_Rect clip,  int frame_width, int frame_height);

#endif
