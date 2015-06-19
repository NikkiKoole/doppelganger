#include "blockmap.h"

//BuildOrder orderArray[2];

void createOrdersFromDimensions(int WIDTH, int HEIGHT, int DEPTH, BuildOrder oa[])
{
    BuildOrder front_order = {
        .first  = X,  .first_start  = 0,      .first_end  = WIDTH,  .first_step  = 1,
        .second = Z,  .second_start = 0,      .second_end = HEIGHT, .second_step = 1,
        .third  = Y,  .third_start  = 0,      .third_end  = DEPTH,  .third_step  = 1
    };

    BuildOrder back_order = {
        .first  = X,  .first_start  = WIDTH,  .first_end  = 0,      .first_step  = -1,
        .second = Z,  .second_start = 0,      .second_end = HEIGHT, .second_step = 1,
        .third  = Y,  .third_start  = DEPTH,  .third_end  = 0,      .third_step  = -1
    };

    oa[0] = front_order;
    oa[1] = back_order;
}
void setBlockAt(World *world, int x, int y, int z, uint8 type)
{
    world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type = type;
}
uint8 getBlockAt(World *world, int x, int y, int z)
{
    return world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type;
}

uint8 getBlockWhileLookingFromSide(World *w, int s, int x, int y, int z)
{
    UNUSED(w);
    UNUSED(x);
    UNUSED(y);
    UNUSED(z);
    //printf("side int: %d", (int) s);
    printf("\n%d\n", orderArray[s].first_start);
    return 0;
}
