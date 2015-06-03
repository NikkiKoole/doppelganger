#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_FRAMES_PER_ANIMATION 128
#include "defines.h"

typedef struct Frame
{
    int32 index;
    int32 duration;
    void* user_data;
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
    int32 n_frames;
    enum Animation_Mode mode;
    int32 loop_from;
    int32 loop_to;

} Animation;

void animation_init(Animation* a);

void animation_add_frame(Animation* a, int32 index, int32 duration, void* data);

#endif
