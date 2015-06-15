#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "defines.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Keyboard
{
    const uint8 *keys;
    int32 states[256];

} Keyboard;

bool key_pressed(Keyboard *keyboard, uint8 key);
bool key_down(Keyboard *keyboard, uint8 key);

extern uint8 KB_SPACE;
extern uint8 KB_RIGHT;
extern uint8 KB_LEFT;
extern uint8 KB_UP;
extern uint8 KB_DOWN;
extern uint8 KB_ESC;
extern uint8 KB_F5;
extern uint8 KB_W;
extern uint8 KB_S;
extern uint8 KB_A;
extern uint8 KB_D;


#endif
