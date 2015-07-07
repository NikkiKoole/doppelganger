#include "memory.h"
#include "blockmap.h"
#include <stdlib.h>
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

void draw_3d_lines(int width, int height, int depth, SDL_Renderer* renderer, Screen* screen)
{
    int x_off = screen->width/2 - ((width*16)/2);
    int y_off = screen->height/2 - ((depth*8 + height*16)/2);
    int x, y, z;
    SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
    for (x = 0; x <= width; x++){
        SDL_RenderDrawLine(renderer,
                           x_off+  x*16,
                           y_off + depth*8 + height*16 ,
                           x_off+ x*16,
                           y_off + 0);
    }
    for (z = 0; z <= height; z++){
        SDL_RenderDrawLine(renderer,
                           x_off + 0,
                           y_off+ z*16 ,
                           x_off+ width*16 ,
                           y_off+ z*16);
    }
    for (y = 0; y <= depth; y++) {
        SDL_RenderDrawLine(renderer,
                           x_off +  0,
                           y_off+ y*8 + height*16,
                           x_off+ width*16,
                           y_off+ y*8 + height*16);
    }

}

void draw_3d_space_helper(int value, Texture *tex, SDL_Renderer *renderer, SDL_Rect source, SDL_Rect dest)
{
    if (value == 1) {
        texture_set_color((tex), 0xFF, 0x00, 0xFF);
    }
    if (value == 2) {
        texture_set_color((tex), 0xFF, 0xFF, 0x00);
    }
    if (value == 3) {
        texture_set_color((tex), 0x00, 0xFF, 0xFF);
    }
    if (value == 4) {
        texture_set_color((tex), 0xff, 0x00, 0x00);
    }
    if (value == 5) {
        texture_set_color((tex), 0x00, 0xFF, 0x00);
    }
    if (value == 6) {
        texture_set_color((tex), 0x00, 0x00, 0xFF);
    }

    if (value > 0){
        texture_render_part(tex, &source, &dest, renderer);
    }
}

void draw_3d_space_in_slices(World *world,
                             Side side,
                             SDL_Renderer *renderer,
                             Screen *screen,
                             Texture *slices,
                             Texture *tex)

{
    UNUSED(world);
    UNUSED(side);
    UNUSED(renderer);
    UNUSED(screen);
    UNUSED(slices);
    UNUSED(tex);

    int my_width = (side == front || side == back) ? world->width : world->height;
    int my_height = (side == front || side == back) ? world->height : world->width;

    //prepare_scratch_bboxes(my_width, my_height, collection);

    // now we will walk the world
    // front to back, column by column
    // and grow the bboxcolumns
    int x_off = 0;
    int y_off = 0;
    // lets just start with the front Side.
    for (int x = 0; x< world->width; x++) {
        for (int z = 0; z<world->height; z++) {
            for (int y = 0; y< world->depth; y++) {
                int value = getBlockAt(world, x, y, z);
                if (value > 0) {
                    int tlX = x*16;
                    int tlY = (world->height*16)  + (y*8) - (z*16) - 16;
                    BBox bb = {{tlX, tlY}, {tlX+16, tlY+24}};
                    char buffer[64];

                    bbox_to_buffer(bb, buffer);
                    printf("this bbox: %s\n", buffer);
                }
            }
            printf("y column done\n");
        }

    }

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

internal void drawLines(World *world, SDL_Renderer *renderer, Screen *screen)
{
    SDL_SetRenderDrawColor( renderer, 0xAA, 0xAA, 0xAA, 0xFF );
    SDL_RenderClear( renderer );
    draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
}

internal void drawWait(SDL_Renderer *renderer)
{
    SDL_RenderPresent( renderer );
    SDL_Delay(40);
}

void draw_3d_space(World *world, Side side, SDL_Renderer *renderer, Screen *screen, Texture *tex)
{
    int x_off;
    int y_off;
    int index = 6;
    SDL_Rect source = {.x=index*16, .y=0, .w=16, .h=24};

    switch(side){
    case(front) :
        x_off = screen->width/2 - ((world->width*16)/2);
        y_off = screen->height/2 - ((world->depth*8 + world->height*16)/2);
        draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
        for (int x = 0; x < world->width; x++) {
            for (int z = 0; z < world->height; z++) {
                for (int y = 0; y< world->depth; y++) {
                    //drawLines(world, renderer, screen);
                    int value = getBlockAt(world, x, y, z);
                    SDL_Rect dest = {.x= x_off + x*16,
                                     .y= y_off + (world->height*16)  + (y*8) - (z*16) - 16,
                                     .w=16, .h=24};
                    draw_3d_space_helper(value, tex, renderer, source, dest);
                    // drawWait(renderer);
                }
            }
        }

        //abort();
        break;
    case(back) :
        x_off = screen->width/2 - ((world->width*16)/2);
        y_off = screen->height/2 - ((world->depth*8 + world->height*16)/2);

        draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
        for (int x = 0; x < world->width; x++) {
            for (int z = 0; z < world->height; z++) {
                for (int y = 0; y< world->depth; y++) {
                    //drawLines(world, renderer, screen);
                    int value = getBlockAt(world, (world->width-1-x), (world->depth-1-y), z);
                    SDL_Rect dest = {.x= x_off + x*16,
                                     .y= y_off + (world->height*16) + (y*8)-(z*16) - 16,
                                     .w=16, .h=24};
                    draw_3d_space_helper(value, tex, renderer, source, dest);
                    //drawWait(renderer);
                }
            }
        }
        //abort();
        break;

    case(left):
        x_off = screen->width/2 - ((world->depth*16)/2);
        y_off = screen->height/2 - ((world->width*8 + world->height*16)/2);
        draw_3d_lines(world->depth, world->height, world->width, renderer, screen);
        for (int y = 0; y< world->depth; y++) {
            for (int z = 0; z < world->height; z++) {
                for (int x = 0; x < world->width; x++) {
                    //drawLines(world, renderer, screen);

                    int value = getBlockAt(world, x, y, z);
                    SDL_Rect dest = {.x= x_off + y*16,
                                     .y= y_off + (world->height*16) + (x*8)-(z*16) - 16,
                                     .w=16, .h=24};

                    draw_3d_space_helper(value, tex, renderer, source, dest);
                    //drawWait(renderer);
                }
            }
        }
        //abort();
        break;
    case(right):
        x_off = screen->width/2 - ((world->depth*16)/2);
        y_off = screen->height/2 - ((world->width*8 + world->height*16)/2);
        draw_3d_lines(world->depth, world->height, world->width, renderer, screen);
        for (int y = 0; y< world->depth; y++) {
            for (int z = 0; z < world->height; z++) {
                for (int x = 0; x < world->width; x++) {
                    //drawLines(world, renderer, screen);

                    int value = getBlockAt(world, (world->width-1-x), y, z);
                    SDL_Rect dest = {.x= x_off + y*16,
                                     .y= y_off + (world->height*16) + (x*8)-(z*16) - 16,
                                     .w=16, .h=24};

                    draw_3d_space_helper(value, tex, renderer, source, dest);
                    //drawWait(renderer);
                }
            }
        }
        //abort();
        break;

    default:
        printf("shouldnt be here");
    }
}
