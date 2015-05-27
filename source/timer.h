#ifndef TIMER_H
#define TIMER_H

#include "defines.h"

typedef struct timer
{
    uint32 start_ticks;
    uint32 paused_ticks;
    bool paused;
    bool started;
} timer;

void timer_init(timer* t);
void timer_stop(timer* t);
void timer_start(timer* t);
void timer_pause(timer* t);
void timer_unpause(timer* t);
uint32 timer_get_ticks(timer* t);

#endif
