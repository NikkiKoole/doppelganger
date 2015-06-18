#ifndef BLOCKMAP__H
#define BLOCKMAP__H

#define WORLD_WIDTH    32
#define WORLD_HEIGHT   32
#define WORLD_DEPTH    32

typedef struct Block
{
    uint8 type;
} Block;

typedef struct World
{
    Block blocks[WORLD_WIDTH  *WORLD_HEIGHT  *WORLD_DEPTH];
} World;

int setBlockAt(Block *blocks, int x, int y, int z, uint8 type);
uint8 getBlockAt(Block *blocks, int x, int y, int z);

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
    int first_step;

    Axis second;
    int second_start;
    int second_end;
    int second_step;

    Axis third;
    int third_start;
    int third_end;
    int third_step;
} BuildOrder;

void cacheSlices(Texture *slices, Block *blocks, BuildOrder order);




#endif
