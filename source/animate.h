#ifndef ANIMATE_H
#define ANIMATE_H

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

#endif
