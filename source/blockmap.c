#include "blockmap.h"

void createOrdersFromDimensions(int width, int height, int depth, BuildOrder *orders)
{
    BuildOrder front  = {
        .first  = X,  .first_start  = 0,      .first_end  = width,
        .second = Z,  .second_start = 0,      .second_end = height,
        .third  = Y,  .third_start  = 0,      .third_end  = depth
    };

    BuildOrder back = {
        .first  = X,  .first_start  = width-1,  .first_end  = 0,
        .second = Z,  .second_start = 0,        .second_end = height,
        .third  = Y,  .third_start  = depth-1,  .third_end  = 0
    };

    BuildOrder left = {
        .first  = Y,   .first_start  = 0,       .first_end  = depth,
        .second = Z,   .second_start = 0,       .second_end = height,
        .third  = X,   .third_start  = width-1, .third_end  = 0
    };

    BuildOrder right = {
        .first  = Y,   .first_start  = depth-1, .first_end  = 0,
        .second = Z,   .second_start = 0,       .second_end = height,
        .third  = X,   .third_start  = 0,       .third_end  = width
    };

    BuildOrder top = {
        .first  = X,   .first_start  = 0,       .first_end  = width,
        .second = Y,   .second_start = depth-1, .second_end = 0,
        .third  = Z,   .third_start  = 0,       .third_end  = height
    };

    BuildOrder bottom = {
        .first  = X,   .first_start  = 0,       .first_end  = width,
        .second = Y,   .second_start = 0,       .second_end = depth,
        .third  = Z,   .third_start  = height-1,.third_end  = 0
    };

    orders[0] = front;
    orders[1] = back;
    orders[2] = left;
    orders[3] = right;
    orders[4] = top;
    orders[5] = bottom;
}
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
    int counters[3];
    // I'll fix the start and end index with a minus one here when negative step
    // TODO: maybe move this out of here and into a buildorder initialize or something.
    //if (order.first_step < 0) { order.first_start -= 1; order.first_end -= 1;}
    //if (order.second_step < 0) { order.second_start -= 1; order.second_end -= 1;}
    //if (order.third_step < 0) { order.third_start -= 1; order.third_end -= 1;}
    //printf("\n%d, %d, %d\n", order.first_step, order.second_step, order.third_step);
    // TODO get the correct x,y,z values to feed getBlockAt
    int first_step = order.first_start < order.first_end ? 1 : -1;
    int second_step = order.second_start < order.second_end ? 1 : -1;
    int third_step = order.third_start < order.third_end ? 1 : -1;

    int x_real = order.first_start + first_step * x;
    int y_real = order.third_start + third_step * y;
    int z_real = order.second_start + second_step * z;
    uint8 value = getBlockAt(w, x_real, y_real, z_real);
    printf("\n %d %d %d value: %d\n",x_real, y_real, z_real, value);
    return value;
}
