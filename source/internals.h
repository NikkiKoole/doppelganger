#ifndef INTERNALS_H
#define INTERNALS_H

#include <SDL2/SDL.h>

typedef struct screen_container
{
    SDL_Renderer* renderer;
    SDL_Window* window;
} screen_container;

#endif
