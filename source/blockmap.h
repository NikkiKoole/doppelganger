#ifndef BLOCKMAP__H
#define BLOCKMAP__H


#include "screen.h"
#include "memory.h"
#include "defines.h"
#include "texture.h"
#include "geom.h"

typedef struct TextureWorldSlice {
    BBox bounds;
    Texture tex;
} TextureWorldSlice;

typedef struct Block
{
    u8 type;
} Block;

typedef struct World
{
    u8 width;
    u8 height;
    u8 depth;
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
void setBlockAt(World *w, int x, int y, int z, u8 type);
u8 getBlockAt(World *w, int x, int y, int z);
void draw_3d_space(World *world, Side side, SDL_Renderer *renderer, Screen *screen, Texture *tex, TransState *trans_state);

#endif
