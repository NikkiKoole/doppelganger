#ifndef TIMER_H
#define TIMER_H

#include "defines.h"

typedef struct Timer
{
    uint32 start_ticks;
    uint32 paused_ticks;
    bool paused;
    bool started;
} Timer;

void timer_init(Timer* t);
void timer_stop(Timer* t);
void timer_start(Timer* t);
void timer_pause(Timer* t);
void timer_unpause(Timer* t);
uint32 timer_get_ticks(Timer* t);

#endif
