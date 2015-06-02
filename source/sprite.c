
#include "sprite.h"

void test(void) 
{
    printf("aoisdjaoids\n");
}
void sprite_init(Sprite* spr, Texture* t, SDL_Rect clip,  int frame_width, int frame_height)
{
    spr->frame_width = 0 + t->width + clip.w + frame_width + frame_height;
    
    printf("Stuff that I should do\n");
}
