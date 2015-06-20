#ifndef BLOCKMAP__H
#define BLOCKMAP__H

#define WORLD_WIDTH    32
#define WORLD_HEIGHT   32
#define WORLD_DEPTH    32

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
    top,
    bottom
} Side;

typedef enum Axis
{
    X,
    Y,
    Z
} Axis;

typedef struct BuildOrder
{
    Axis first;
    int first_start;
    int first_end;

    Axis second;
    int second_start;
    int second_end;

    Axis third;
    int third_start;
    int third_end;
} BuildOrder;


void setBlockAt(World *w, int x, int y, int z, uint8 type);
uint8 getBlockAt(World *w, int x, int y, int z);
uint8 getBlockWhileLookingFromSide(World *w, BuildOrder b, int x, int y, int z);
void createOrdersFromDimensions(int WIDTH, int HEIGHT, int DEPTH, BuildOrder *orderArray);

void cacheSlices(Texture *slices, Block *blocks, BuildOrder order);




#endif
