#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>


typedef struct Screen
{
    int width;
    int height;
    SDL_Renderer *renderer;
    SDL_Window *window;
} Screen;


#endif
