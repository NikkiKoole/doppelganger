
#include "sprite.h"
#include "defines.h"

void sprite_init(Sprite* spr, Texture* t, SDL_Rect clip,  int frame_width, int frame_height)
{
    spr->texture = t;
    spr->clip = clip;
    spr->frame_width = frame_width;
    spr->frame_height = frame_height;
    printf("frame columns in sprite: %d\n",clip.w/frame_width);
    printf("frame rows in sprite: %d\n",clip.h/frame_height);
}

void sprite_play_animation(Sprite* spr, Animation* anim)
{
    UNUSED(spr);
    UNUSED(anim);
}
