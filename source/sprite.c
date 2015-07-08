
#include "sprite.h"
#include "defines.h"


void sprite_init(Sprite* spr, Texture* t, SDL_Rect clip,  int32 frame_width, int32 frame_height)
{
    spr->texture = t;
    spr->clip = clip;
    spr->frame_width = frame_width;
    spr->frame_height = frame_height;
    spr->elapsed_time = 0;
    spr->current_frame = 0;
    printf("frame columns in sprite: %d\n",clip.w/frame_width);
    printf("frame rows in sprite: %d\n",clip.h/frame_height);
}

SDL_Rect sprite_get_current_frame(Sprite* spr, Animation* anim)
{
    int32 d = anim->frames[spr->current_frame].duration;
    if (spr->elapsed_time > d) {
        if (spr->current_frame < anim->n_frames) {
            spr->current_frame++;
        }
        else {
            spr->current_frame = 0;
        }
        spr->elapsed_time = d - spr->elapsed_time;
    }
    int xs = spr->clip.x + spr->frame_width * spr->current_frame;
    int ys = spr->clip.y;
    SDL_Rect result = {.x=xs, .y=ys, .w=spr->frame_width , .h=spr->frame_height};
    return result;
}

void sprite_update_elapsed_time(Sprite* spr, int32 duration) {
    spr->elapsed_time += duration;
}
