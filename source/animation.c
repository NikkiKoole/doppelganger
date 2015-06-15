#include "animation.h"

void animation_init(Animation *a)
{
    a->mode = LOOP_FRAMES;
    a->loop_from = -1;
    a->loop_to = -1;
}

void animation_add_frame(Animation *a, int32 index, int32 duration, void *data)
{
    int32 n = a->n_frames;
    if (n < MAX_FRAMES_PER_ANIMATION) {
        a->frames[n].index = index;
        a->frames[n].duration = duration;
        a->frames[n].user_data = data;
        if (a->mode == LOOP_FRAMES) {
            if (a->loop_from == -1) {
                a->loop_from = 0;
            }
            if (a->loop_to == -1 || a->loop_to == a->n_frames -1) {
                a->loop_to = a->n_frames;
            }
        }
        a->n_frames++;
    }
}
