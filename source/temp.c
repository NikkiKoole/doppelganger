#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH      120
#define HEIGHT     80
#define DEPTH      80
#define MAX_ACTORS 3000
#define u8         uint8_t
#define u16        uint16_t

typedef struct Actor
{
    int x;
    int y;
    int z;
    int frame;
    long data;
    long data2;
} Actor;

typedef struct Tile
{
    u8 color;
    u8 light;
    u16 texture;
} Tile;

typedef struct World
{
    Tile tiles[WIDTH*HEIGHT*DEPTH];
    Actor actors[MAX_ACTORS];
} World;

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

void build(BuildOrder order)
{

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
    // I'll use iterator names according to this image, the actual axis depend on the BuildOrder

    int counters[3];
    // I'll fix the start and end index with a minus one here when negative step
    // TODO: maybe move this out of here and into a buildorder initialize or something.

    if (order.first_step < 0) { order.first_start -= 1; order.first_end -= 1;}
    if (order.second_step < 0) { order.second_start -= 1; order.second_end -= 1;}
    if (order.third_step < 0) { order.third_start -= 1; order.third_end -= 1;}

    for (int y = order.third_start; y != order.third_end; y += order.third_step ){
        for (int z = order.second_start; z != order.second_end; z += order.second_step) {
            for (int x = order.first_start; x != order.first_end; x += order.first_step) {
                counters[order.first]  = x;
                counters[order.second] = y;
                counters[order.third]  = z;
                //(y*(WIDTH*HEIGHT)) + (z*WIDTH) + x )
                //printf("%d \n", (counters[2]*(WIDTH*HEIGHT)) + (counters[1]*WIDTH) + counters[0]);
            }
        }
    }
}

int mainOld() {
    printf("%lu kB.\n", sizeof(World) / 1000);

    World world = {};

    BuildOrder front = {
        .first  = X,  .first_start  = 0,      .first_end  = WIDTH,  .first_step  = 1,
        .second = Z,  .second_start = 0,      .second_end = HEIGHT, .second_step = 1,
        .third  = Y,  .third_start  = 0,      .third_end  = DEPTH,  .third_step  = 1
    };

    BuildOrder back = {
        .first  = X,  .first_start  = WIDTH,  .first_end  = 0,      .first_step  = -1,
        .second = Z,  .second_start = 0,      .second_end = HEIGHT, .second_step = 1,
        .third  = Y,  .third_start  = DEPTH,  .third_end  = 0,      .third_step  = -1
    };

    clock_t start = clock() ;
    build(back);
    build(front);
    clock_t end = clock() ;
    double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;
    printf("Building took: %f \n", elapsed_time);



}
