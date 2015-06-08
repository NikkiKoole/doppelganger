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
    u8 texture;
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




void fillTile(Tile *tiles, int x, int y, int z, int value) 
{
    Tile v = {.texture=(u8)value};
    tiles[(y*(WIDTH*HEIGHT)) + (z*WIDTH) + x ] = v;
}

u8 getValueAt(Tile *tiles, int x, int y, int z)
{
    return tiles[(y*(WIDTH*HEIGHT)) + (z*WIDTH) + x ].texture;
}

/*
  When culling a 3d world what do I do?  I check the world front -->
  back So I start with Y = Max and go to Y = Min at each iteration I
  don't change the x, but do check all y values.  if the tile I am
  currenlty checking out (bounding box collides)/(fully overlaps) with
  any of the earlier ones or set of earlier ones, I dont draw it.

  This imples I should keep a list of rectangles that are drawn at the
  current X column.
  This in turn leads me to an algorithm that should be able to possible grow rectangle
 */

typedef struct Shape
{
    int x;
    int y;
    int w;
    int h;
} Shape;

enum Overlap
{
    NONE,
    PARTLY,
    FULL
};

enum Overlap shape_overlaps(Shape *test, Shape *existing)
{
    int pleft = test->x;
    int pright = pleft + test->w;
    int ptop = test->y;
    int pbottom = ptop + test->h;
    
    int eleft = existing->x;
    int eright = eleft + existing->w;
    int etop = existing->y;
    int ebottom = etop + existing->h;

    if (pright < eleft || pleft > eright || pbottom < etop || ptop > ebottom)
        return NONE;
    if (pbottom < ebottom || ptop > etop || pright < eright || pleft > eleft) 
        return PARTLY;
    return FULL;
}

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b);\
        _a > _b ? _a : _b; })

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b);\
        _a < _b ? _a : _b; })


Shape combine_shapes(Shape *add, Shape *existing)
{
    Shape result = {.x = existing->x, .y = existing->y};
    int growWidth = 0;
    int growHeight = 0;

    if (add->x < existing->x){ 
        result.x  = add->x;
        growWidth = existing->x - add->x;
    } 
    if (add->y < existing->y){ 
        result.y  = add->y;
        growHeight = existing->y - add->y;
    } 

    printf("some min test %d", MIN(-12020, 123123));

    result.w = existing->w + growWidth; 
    result.h = existing->h + growHeight;
    return(result);
}


int main() {
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

    fillTile(world.tiles, 0, 0, 0, 99);
    u8 value = getValueAt(world.tiles, 0, 0, 0);
    printf("retrieved %d from 3d array.\n", (u8)value);

    Shape existing = {.x=0, .y=-24, .w=16, .h=24};
    Shape test = {.x=0, .y=0, .w=16, .h=24};
    if (shape_overlaps(&test, &existing) == PARTLY) {
        existing = combine_shapes(&test, &existing);
    };
    printf("Adding results in Shape(x:%d, y:%d, w:%d, h:%d).\n",existing.x, existing.y, existing.w, existing.h);
}

