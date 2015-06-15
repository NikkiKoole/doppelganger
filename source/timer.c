#include "timer.h"
#include <SDL2/SDL.h>

void timer_init(Timer *t)
{
    t->start_ticks = 0;
    t->paused_ticks = 0;
    t->paused = false;
    t->started = false;
}

void timer_stop(Timer *t)
{
    t->started = false;
    t->paused = false;
    t->start_ticks = 0;
    t->paused_ticks = 0;
}
void timer_start(Timer *t)
{
    t->started = true;
    t->paused = false;
    t->start_ticks = SDL_GetTicks();
    t->paused_ticks = 0;
}
void timer_pause(Timer *t)
{
    if (t->started && !t->paused){
        t->paused = true;
        t->paused_ticks = SDL_GetTicks() - t->start_ticks;
        t->start_ticks = 0;
    }
}
void timer_unpause(Timer *t)
{
    if (t->started && t->paused){
        t->paused = false;
        t->start_ticks = SDL_GetTicks() - t->paused_ticks;
        t->paused_ticks = 0;
    }
}
uint32 timer_get_ticks(Timer *t)
{
    uint32 time = 0;

    if (t->started){
        if (t->paused){
            time = t->paused_ticks;
        } else {
            time = SDL_GetTicks() - t->start_ticks;
        }
    }
    return time;
}

bool timer_is_started(Timer *t)
{
    return t->started;
}
bool timer_is_paused(Timer *t)
{
    return t->paused;
}
