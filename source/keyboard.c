#include "keyboard.h"
#include <SDL2/SDL.h>

uint8 KB_SPACE = SDL_SCANCODE_SPACE;
uint8 KB_RIGHT = SDL_SCANCODE_RIGHT;
uint8 KB_LEFT  = SDL_SCANCODE_LEFT;
uint8 KB_UP    = SDL_SCANCODE_UP;
uint8 KB_DOWN  = SDL_SCANCODE_DOWN;
uint8 KB_ESC   = SDL_SCANCODE_ESCAPE;
uint8 KB_F5    = SDL_SCANCODE_F5;
uint8 KB_W     = SDL_SCANCODE_W;
uint8 KB_S     = SDL_SCANCODE_S;
uint8 KB_A     = SDL_SCANCODE_A;
uint8 KB_D     = SDL_SCANCODE_D;

int key_pressed(Keyboard* keyboard , uint8 key ) {
    if ( keyboard->keys[ key ] ) {
        if ( keyboard->states[ key ] == 0 ) {
            keyboard->states[ key ] = 1;
            return 1;
        }
    } else {
        keyboard->states[ key ] = 0;
    }
    return 0;
}

int key_down(Keyboard* keyboard, uint8 key ) {
    return ( keyboard->keys[ key ] );
}
