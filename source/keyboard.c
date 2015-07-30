#include "keyboard.h"
#include <SDL2/SDL.h>

u8 KB_SPACE = SDL_SCANCODE_SPACE;
u8 KB_RIGHT = SDL_SCANCODE_RIGHT;
u8 KB_LEFT  = SDL_SCANCODE_LEFT;
u8 KB_UP    = SDL_SCANCODE_UP;
u8 KB_DOWN  = SDL_SCANCODE_DOWN;
u8 KB_ESC   = SDL_SCANCODE_ESCAPE;
u8 KB_F5    = SDL_SCANCODE_F5;
u8 KB_W     = SDL_SCANCODE_W;
u8 KB_S     = SDL_SCANCODE_S;
u8 KB_A     = SDL_SCANCODE_A;
u8 KB_D     = SDL_SCANCODE_D;

b32 key_pressed(Keyboard *keyboard , u8 key ) {
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

b32 key_down(Keyboard *keyboard, u8 key ) {
    return ( keyboard->keys[ key ] );
}
