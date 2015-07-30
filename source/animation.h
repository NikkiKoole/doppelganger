#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_FRAMES_PER_ANIMATION 128
#include "defines.h"

typedef struct Frame
{
    s32 index;
    s32 duration;
    void *user_data;
} Frame;

enum Animation_Mode
{
    LOOP_FRAMES,
    PINGPONG_FRAMES,
    FREEZE_LAST_FRAME
};

typedef struct Animation
{
    Frame frames[MAX_FRAMES_PER_ANIMATION];
    s32 n_frames;
    enum Animation_Mode mode;
    s32 loop_from;
    s32 loop_to;

} Animation;

void animation_init(Animation *a);

void animation_add_frame(Animation *a, s32 index, s32 duration, void *data);

#endif
