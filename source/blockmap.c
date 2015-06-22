#include "blockmap.h"

/*
    |Z (height)
    |
    |
    |
    |
    /---------X (width)
   /
  /Y (depth)

*/

void resetBlocks(World *world)
{
    for (int i = 0; i< world->width*world->height*world->depth; i++) {
        world->blocks[i].type = 0;
    }
}



void setBlockAt(World *world, int x, int y, int z, uint8 type)
{
    ASSERT(x < world->width);
    ASSERT(y < world->depth);
    ASSERT(z < world->height);

    world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type = type;
}
uint8 getBlockAt(World *world, int x, int y, int z)
{
    return world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type;
}
