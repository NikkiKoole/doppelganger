#include "memory.h"
#include "blockmap.h"
#include "geom.h"
#include <stdlib.h>
#include "linkedlist.h"

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

void setBlockAt(World *world, int x, int y, int z, u8 type)
{
    ASSERT(x >= 0); ASSERT(x < world->width);
    ASSERT(y >= 0); ASSERT(y < world->depth);
    ASSERT(z >= 0); ASSERT(z < world->height);

    world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type = type;
}
u8 getBlockAt(World *world, int x, int y, int z)
{
    return world->blocks[(y * (world->width * world->height)) + (z * world->width) + x].type;
}

internal void draw_3d_lines(int width, int height, int depth, SDL_Renderer* renderer, Screen* screen)
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

internal void drawLines(World *world, SDL_Renderer *renderer, Screen *screen, int frontal)
{
    SDL_SetRenderDrawColor( renderer, 0xAA, 0xAA, 0xAA, 0xFF );
    SDL_RenderClear( renderer );
    if (frontal) {
        draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
    } else {
        draw_3d_lines(world->depth, world->height, world->width, renderer, screen);
    }
}

internal void drawWait(SDL_Renderer *renderer)
{
    SDL_RenderPresent( renderer );
    SDL_Delay(40);
}


internal void printList(List *list) {
    LIST_FOREACH(list, first, next, cur) {
        BBox *v = cur->value;
        printBBox(*v);
    }
}

internal b32 growList(List *list, BBox *current, ListNode *node) {

    int fully_contained_by = 0;
    int partly_overlapped_by = 0;
    int barely_touched_by = 0;

    LIST_FOREACH(list, first, next, cur) {
        BBox *v = cur->value;
        BBox result;
        int i = bbox_intersect(*current, *v, &result);

        if (!bbox_eql(*current, *v)) {
            if (bbox_in_bbox(*current, *v)) {
                fully_contained_by = 1;
                goto decided;
            } else if (i) {
                bbox_grow_vertically(v, *current);
                partly_overlapped_by = 1;
                goto decided;
            } else if (bbox_neighbour_vertically(*v, *current)) {
                bbox_grow_vertically(v, *current);
                barely_touched_by = 1;
                goto decided;
            }
        }
    }
 decided:
    if (! fully_contained_by) {
        if (barely_touched_by || partly_overlapped_by) {
            return true;

        }  else {
            list_add_last(list, node);
            return false;
        }
    }
    return false;
}

inline internal void reset_list(List *list)
{
    list->length = 0;
    list->first = NULL;
    list->last = NULL;
}



internal void draw_3d_space_helper(int value, Texture *tex, SDL_Renderer *renderer, SDL_Rect source, SDL_Rect dest)
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


internal void handleFilledBlock(TransState *trans_state, SDL_Rect source, SDL_Rect dest, BBox *bbox_slices, List *list, int value, Texture *tex, SDL_Renderer *renderer, int index)
{
    BBox *current = (BBox*) PUSH_STRUCT(&trans_state->scratch_arena, BBox);
    current->tl.x = dest.x;
    current->tl.y = dest.y;
    current->br.x  = dest.x+dest.w;
    current->br.y = dest.y+dest.h;
    void *val =  current;
    ListNode *node = (ListNode*) PUSH_STRUCT(&trans_state->scratch_arena, ListNode);
    node->value = val;

    if (growList(list, current, node)) {

        //int outcome = 1 + (int)(randFloat() * 6);
        int outcome = value;//randInt(1, 7);
        //printf("%d\n",outcome);
        draw_3d_space_helper(outcome, tex, renderer, source, dest);
        bbox_grow(&bbox_slices[index], *current);
    }
}

internal void prepare_memory( TransState *trans_state, TempMemory *scratch, List **list,  BBox **bbox_slices, int amount)
{
    UNUSED(list);UNUSED(amount);
    *scratch = begin_temporary_memory(&trans_state->scratch_arena);
    *list = (List*) PUSH_STRUCT(&trans_state->scratch_arena, List);
    *bbox_slices = (BBox*) PUSH_ARRAY(&trans_state->scratch_arena, amount, BBox ); //the actual depth at this view

}

internal b32 in_screen_bounds(SDL_Rect dest, Screen *screen)
{
    return (dest.x+dest.w > 0 &&
            dest.x < screen->width &&
            dest.y+dest.h > 0 &&
            dest.y < screen->height);
}


void draw_3d_space(World *world, Side side, SDL_Renderer *renderer, Screen *screen, Texture *tex, TransState *trans_state, CachedSlices *cached)
{
    int x_off;
    int y_off;
    int index = 6;
    SDL_Rect source = {.x=index*16, .y=0, .w=16, .h=24};
    TempMemory scratch;
    List *list = 0;
    BBox *bbox_slices = 0;
    int this_depth = 0;

    switch(side){
    case(front) :
        x_off = screen->width/2 - ((world->width*16)/2);
        y_off = screen->height/2 - ((world->depth*8 + world->height*16)/2);
        draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
        this_depth = world->depth;

        scratch = begin_temporary_memory(&trans_state->scratch_arena);
        list = (List*) PUSH_STRUCT(&trans_state->scratch_arena, List);
        bbox_slices = (BBox*) PUSH_ARRAY(&trans_state->scratch_arena, this_depth, BBox ); //the actual depth at this view
        for (int i = 0; i<this_depth; i++ ) {
            bbox_slices[i] = (BBox) {{1000000,1000000},{0,0}};
        }
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );

        for (int x = 0; x < world->width; x++) {

            reset_list(list);
            //for (int y = 0; y< world->depth; y++) {
            for (int y = world->depth-1; y>=0; y--) {

                texture_set_blend_mode(&cached->slices[y].tex, SDL_BLENDMODE_BLEND);
                texture_set_as_rendertarget(&cached->slices[y].tex, renderer);

                for (int z = 0; z < world->height; z++) {

                    int value = getBlockAt(world, x, y, z);
                    SDL_Rect dest = {.x= x_off + x*16,
                                     .y= y_off + (world->height*16)  + (y*8) - (z*16) - 16,
                                     .w=16, .h=24};

                    if (value > 0 && in_screen_bounds(dest, screen)) {
                        handleFilledBlock(trans_state, source, dest, bbox_slices, list, value, tex, renderer, y);
                    }
                } // z
            } // y
        } // x

        for (int i = 0; i<this_depth; i++ ) {
            cached->slices[i].bounds = bbox(bbox_slices[i].tl.x,bbox_slices[i].tl.y,bbox_slices[i].br.x, bbox_slices[i].br.y);
            //printBBox(cached->slices[i].bounds);
        }

        //now we can do something usefull with the bbox_slices

        end_temporary_memory(scratch);
        //abort();
        break;
    case(back) :
        x_off = screen->width/2 - ((world->width*16)/2);
        y_off = screen->height/2 - ((world->depth*8 + world->height*16)/2);
        draw_3d_lines(world->width, world->height, world->depth, renderer, screen);
        this_depth = world->depth;

        scratch = begin_temporary_memory(&trans_state->scratch_arena);
        list = (List*) PUSH_STRUCT(&trans_state->scratch_arena, List);
        bbox_slices = (BBox*) PUSH_ARRAY(&trans_state->scratch_arena, this_depth, BBox ); //the actual depth at this view
        for (int i = 0; i<this_depth; i++ ) {
            bbox_slices[i] = (BBox) {{1000000,1000000},{0,0}};
        }

        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );


        for (int x = 0; x < world->width; x++) {
            reset_list(list);
            //for (int y = 0; y< world->depth; y++) {
            for (int y = world->depth-1; y>=0; y--) {
                texture_set_blend_mode(&cached->slices[y].tex, SDL_BLENDMODE_BLEND);
                texture_set_as_rendertarget(&cached->slices[y].tex, renderer);
                for (int z = 0; z < world->height; z++) {
                    int value = getBlockAt(world, (world->width-1-x), (world->depth-1-y), z);
                    SDL_Rect dest = {.x= x_off + x*16,
                                     .y= y_off + (world->height*16) + (y*8)-(z*16) - 16,
                                     .w=16, .h=24};
                    if (value > 0 && in_screen_bounds(dest, screen)) {
                        handleFilledBlock(trans_state, source, dest, bbox_slices, list, value, tex, renderer, y);
                    }
                }
            }
        }
        end_temporary_memory(scratch);
        break;
    case(left):
        x_off = screen->width/2 - ((world->depth*16)/2);
        y_off = screen->height/2 - ((world->width*8 + world->height*16)/2);
        draw_3d_lines(world->depth, world->height, world->width, renderer, screen);
        this_depth = world->width;

        scratch = begin_temporary_memory(&trans_state->scratch_arena);
        list = (List*) PUSH_STRUCT(&trans_state->scratch_arena, List);
        bbox_slices = (BBox*) PUSH_ARRAY(&trans_state->scratch_arena,this_depth, BBox ); //the actual depth at this view
        for (int i = 0; i<this_depth; i++ ) {
            bbox_slices[i] = (BBox) {{1000000,1000000},{0,0}};
        }

        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );

        for (int y = 0; y< world->depth; y++) {
            reset_list(list);
            //for (int x = 0; x < world->width; x++) {
            for (int x = world->width-1; x >= 0; x--) {
                texture_set_blend_mode(&cached->slices[x].tex, SDL_BLENDMODE_BLEND);
                texture_set_as_rendertarget(&cached->slices[x].tex, renderer);
                for (int z = 0; z < world->height; z++) {
                    int value = getBlockAt(world, (world->width-1-x), y, z);
                    SDL_Rect dest = {.x= x_off + y*16,
                                     .y= y_off + (world->height*16) + (x*8)-(z*16) - 16,
                                     .w=16, .h=24};
                    if (value > 0 && in_screen_bounds(dest, screen)) {
                        handleFilledBlock(trans_state, source, dest, bbox_slices, list, value, tex, renderer, x);
                    }
                }
            }
        }
        end_temporary_memory(scratch);
        break;

    case(right):

        x_off = screen->width/2 - ((world->depth*16)/2);
        y_off = screen->height/2 - ((world->width*8 + world->height*16)/2);
        draw_3d_lines(world->depth, world->height, world->width, renderer, screen);
        this_depth = world->width;

        scratch = begin_temporary_memory(&trans_state->scratch_arena);
        list = (List*) PUSH_STRUCT(&trans_state->scratch_arena, List);
        bbox_slices = (BBox*) PUSH_ARRAY(&trans_state->scratch_arena,this_depth, BBox ); //the actual depth at this view
        for (int i = 0; i<this_depth; i++ ) {
            bbox_slices[i] = (BBox) {{1000000,1000000},{0,0}};
        }

        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );

        for (int y = 0; y< world->depth; y++) {
            reset_list(list);
            for (int x = world->width-1; x >= 0; x--) {
                //for (int x = 0; x < world->width; x++) {
                texture_set_blend_mode(&cached->slices[x].tex, SDL_BLENDMODE_BLEND);
                texture_set_as_rendertarget(&cached->slices[x].tex, renderer);
                for (int z = 0; z < world->height; z++) {
                    int value = getBlockAt(world, x, (world->depth-1-y), z);
                    SDL_Rect dest = {.x= x_off + y*16,
                                     .y= y_off + (world->height*16) + (x*8)-(z*16) - 16,
                                     .w=16, .h=24};
                    if (value > 0 && in_screen_bounds(dest, screen)) {
                        handleFilledBlock(trans_state, source, dest, bbox_slices, list, value, tex, renderer, x);
                    }
                }
            }
        }
        end_temporary_memory(scratch);
        break;

    default:
        printf("shouldnt be here");
    }
}
