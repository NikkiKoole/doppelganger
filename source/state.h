#ifndef STATE_H
#define STATE_H

#include "defines.h"
#include "timer.h"
#include "animation.h"
#include "sprite.h"
#include "keyboard.h"
#include "screen.h"
#include "blockmap.h"
#include "geom.h"

typedef struct CachedSlices {
    Point screen_dim;
    TextureWorldSlice *slices;
} CachedSlices;

typedef struct State
{

    Memory_Arena world_arena;
    r64  angle1;
    Texture *tex1;
    Texture *terminal8;
    Texture *render_target;
    Texture *zelda;
    Texture *blocks;
    Timer *timer;
    Animation *animation1;
    Sprite *walking_left;
    Sprite *walking_right;
    World *world;
    Texture *world_slices;
    CachedSlices *cached;

} State;

// Lets imagine many helper functions to grow here

#endif
