#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "defines.h"
#include <SDL2/SDL.h>


typedef struct Keyboard
{
    const u8 *keys;
    s32 states[256];

} Keyboard;

b32 key_pressed(Keyboard *keyboard, u8 key);
b32 key_down(Keyboard *keyboard, u8 key);

extern u8 KB_SPACE;
extern u8 KB_RIGHT;
extern u8 KB_LEFT;
extern u8 KB_UP;
extern u8 KB_DOWN;
extern u8 KB_ESC;
extern u8 KB_F5;
extern u8 KB_W;
extern u8 KB_S;
extern u8 KB_A;
extern u8 KB_D;


#endif
