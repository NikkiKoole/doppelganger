#ifndef TIMER_H
#define TIMER_H

#include "defines.h"


typedef struct Timer
{
    u32 start_ticks;
    u32 paused_ticks;
    b32 paused;
    b32 started;
} Timer;

void timer_init(Timer *t);
void timer_stop(Timer *t);
void timer_start(Timer *t);
void timer_pause(Timer *t);
void timer_unpause(Timer *t);
u32 timer_get_ticks(Timer *t);
b32 timer_is_started(Timer *t);
b32 timer_is_paused(Timer *t);
#endif
