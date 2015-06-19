#include "blockmap.h"


void createOrdersFromDimensions(int WIDTH, int HEIGHT, int DEPTH, BuildOrder *oa)
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

uint8 getBlockWhileLookingFromSide(World *w, BuildOrder order, int x, int y, int z)
{
    UNUSED(w);
    UNUSED(x);
    UNUSED(y);
    UNUSED(z);
    //UNUSED(b);

    int counters[3];
    // I'll fix the start and end index with a minus one here when negative step
    // TODO: maybe move this out of here and into a buildorder initialize or something.

    if (order.first_step < 0) { order.first_start -= 1; order.first_end -= 1;}
    if (order.second_step < 0) { order.second_start -= 1; order.second_end -= 1;}
    if (order.third_step < 0) { order.third_start -= 1; order.third_end -= 1;}
    printf("\n%d, %d, %d\n", order.first_step, order.second_step, order.third_step);



    //for (int y = order.third_start; y != order.third_end; y += order.third_step ){
    //    for (int z = order.second_start; z != order.second_end; z += order.second_step) {
    //        for (int x = order.first_start; x != order.first_end; x += order.first_step) {




    return 100;
}
