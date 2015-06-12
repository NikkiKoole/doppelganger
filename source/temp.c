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

typedef struct Bounds
{
    int left;
    int right;
    int top;
    int bottom;
} Bounds;



Bounds getBoundsForShape(Shape *s)
{
    return (Bounds){.left=s->x, .top=s->y, .right=s->x+s->w, .bottom=s->y+s->h };
}

enum Overlap
{
    NONE,
    PARTLY,
    FULL
};


/*
static internal Rectangle intersect(Rectangle lhs, Rectangle rhs)
{
    Dimension lhsLeft = lhs.Location.X;
    Dimension rhsLeft = rhs.Location.X;
    Dimension lhsTop = lhs.Location.Y;
    Dimension rhsTop = rhs.Location.Y;
    Dimension lhsRight = lhs.Right;
    Dimension rhsRight = rhs.Right;
    Dimension lhsBottom = lhs.Bottom;
    Dimension rhsBottom = rhs.Bottom;

    Dimension left = Dimension.max(lhsLeft, rhsLeft);
    Dimension top = Dimension.max(lhsTop, rhsTop);
    Dimension right = Dimension.min(lhsRight, rhsRight);
    Dimension bottom = Dimension.min(lhsBottom, rhsBottom);
    Point location = new Point(left, top);
    Dimension width = (right > left) ? (right - left) : new Dimension(0);
    Dimension height = (bottom > top) ? (bottom - top) : new Dimension(0);

    return new Rectangle(location, new Size(width, height));
}
 */






Shape getInterSectingShape(Shape *lhs, Shape *rhs)
{
    int lhs_left = lhs->x;
    int lhs_top = lhs->y;
    int lhs_right = lhs_left + lhs->w;
    int lhs_bottom = lhs_top + lhs->h;

    int rhs_left = rhs->x;
    int rhs_top = rhs->y;
    int rhs_right = rhs_left + rhs->w;
    int rhs_bottom = rhs_top + rhs->h;

    int left = MAX(lhs_left, rhs_left);
    int top = MAX(lhs_top, rhs_top);
    int right = MIN(lhs_right, rhs_right);
    int bottom = MIN(lhs_bottom, rhs_bottom);

    int width = (right > left) ? (right - left) : 0;
    int height = (bottom > top) ? (bottom - top) : 0;
    return (Shape){.x=left, .y=top, .w=width, .h=height};
}

enum Overlap shape_overlaps(Shape *lhs, Shape *rhs)
{
    int lhs_left = lhs->x;
    int lhs_right = lhs_left + lhs->w;
    int lhs_top = lhs->y;
    int lhs_bottom = lhs_top + lhs->h;

    int rhs_left = rhs->x;
    int rhs_right = rhs_left + rhs->w;
    int rhs_top = rhs->y;
    int rhs_bottom = rhs_top + rhs->h;

    if (lhs_right < rhs_left || lhs_left > rhs_right || lhs_bottom < rhs_top || lhs_top > rhs_bottom)
        return NONE;
    if (lhs_bottom < rhs_bottom || lhs_top > rhs_top || lhs_right < rhs_right || lhs_left > rhs_left)
        return PARTLY;
    return FULL;
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

    Shape existing = {.x=0, .y=0, .w=16, .h=24};
    Shape test = {.x=4, .y=12, .w=12, .h=20};
    Shape overlap;

    if (shape_overlaps(&test, &existing) == PARTLY) {
        overlap = getInterSectingShape(&test, &existing);
        // the overlap could be of 0 height, this just means they exactly are next to each other
    };
    printf("overlap =(x:%d, y:%d, w:%d, h:%d).\n",overlap.x, overlap.y, overlap.w, overlap.h);
}
