#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_FRAMES_PER_ANIMATION 128

typedef struct Frame 
{   
    int index;
    int duration;
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
    int n_frames;
    enum Animation_Mode mode;
    int loop_from;
    int loop_to;
    
} Animation;

void animation_init(Animation* a);

void animation_add_frame(Animation* a, int index, int duration, void* data);

#endif
