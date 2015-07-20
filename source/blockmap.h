#ifndef BLOCKMAP__H
#define BLOCKMAP__H


#include "screen.h"
#include "memory.h"
#include "defines.h"
#include "texture.h"

typedef struct Block
{
    uint8 type;
} Block;

typedef struct World
{
    uint8 width;
    uint8 height;
    uint8 depth;
    Block *blocks;//[WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH];
} World;

typedef enum Side
{
    front,
    back,
    left,
    right,
} Side;

void resetBlocks(World *world);
void setBlockAt(World *w, int x, int y, int z, uint8 type);
uint8 getBlockAt(World *w, int x, int y, int z);
void draw_3d_space(World *world, Side side, SDL_Renderer *renderer, Screen *screen, Texture *tex, TransState *trans_state);

#endif
