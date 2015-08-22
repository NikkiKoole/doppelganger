#ifndef STATE_H
#define STATE_H

#include "defines.h"
#include "timer.h"
#include "animation.h"
#include "sprite.h"
#include "keyboard.h"
#include "screen.h"
#include "geom.h"
#include "blockmap.h"
#include "entity.h"

typedef struct State
{

    Memory_Arena world_arena;
    r64  angle1;
    Side last_side;
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
    Entity *game_entities;
} State;

// Lets imagine many helper functions to grow here

#endif
