#include <SDL2/SDL.h>
#include "memory.h"
#include "state.h"
#include "texture.h"
#include "timer.h"
#include "animation.h"
#include "keyboard.h"
#include "geom.h"
#include "colors.h"
#include "blockmap.h"
#include <time.h>

char texture1[] = "resources/image.png";
char terminal8[] = "resources/terminal8.png";
char zelda[] = "resources/link.png"; // 24 x 26 px
char blocks[] = "resources/blocks.png"; // 16 x 24 px

#define entity_count 500

internal void initialize_memory(State *state,  Memory* memory, SDL_Renderer* renderer, Screen* screen);
internal void create_slice(State *state, SDL_Renderer* renderer  );
void game_update_and_render(Screen *screen, Memory *memory, Keyboard *keyboard, FrameTime *frametime);

//void set_structured_values_in_world(World* world);


internal void set_structured_values_in_world(World* world)
{


    // bottom plateau
    for (int x = 0; x < world->width; x++) {
        for (int y = 0 ; y < world->depth; y++) {
            setBlockAt(world, x,y,0,5);
        }
    }
    // back wall
    for (int x = 0; x < world->width; x++) {
        for (int z = 0 ; z < world->height; z++) {
            setBlockAt(world, x,0,z,2);
        }
    }
    // left wall
    for (int y = 0; y < world->depth; y++) {
        for (int z = 0 ; z < world->height; z++) {
            //setBlockAt(world, 0,y,z,3);
        }
    }
    // front left column
    for (int z = 0 ; z < world->height; z++) {
        setBlockAt(world, 0,world->depth-1,z,3);
    }

    // right wall
    for (int y = 0; y < world->depth; y++) {
        for (int z = 0 ; z < world->height; z++) {
            //setBlockAt(world, world->width-1,y,z,4);
        }
    }
}

internal int getSliceCount(Side side, World *world) {
    if (side == left || side == right) {
        return world->width;
    }
    return world->depth;
}

extern void game_update_and_render(Screen* screen, Memory* memory, Keyboard* keyboard, FrameTime* frametime)
{
    SDL_Renderer* renderer = screen->renderer;
    ASSERT(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;
    ASSERT(sizeof(TransState) <= memory->transient_storage_size);
    TransState *trans_state = (TransState *)memory->transient_storage;

    Side side_to_render = front;
    int sliceCount;


    if (!memory->is_initialized) {

        initialize_memory(state, memory, renderer, screen);

        initialize_arena(&trans_state->scratch_arena,
                         memory->transient_storage_size - sizeof(TransState),
                         (u8 *)memory->transient_storage + sizeof(TransState));

        memory->is_initialized = true;

        resetBlocks(state->world);
        set_structured_values_in_world(state->world);
        SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
        SDL_RenderClear( renderer );

        for (int i = 0; i < getSliceCount(side_to_render, state->world); i++) {
            texture_set_as_rendertarget(&state->cached->slices[i].tex, renderer);
            texture_set_blend_mode(&state->cached->slices[i].tex, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
            SDL_RenderClear( renderer );
        }

        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );

        draw_3d_space(state->world, side_to_render, renderer, screen, state->blocks, trans_state, state->cached);
        SDL_SetRenderTarget( renderer, NULL );
    }

    if (key_pressed(keyboard,KB_F5)){
        ASSERT(state->tex1);
        texture_load_from_file((state->tex1), texture1, renderer);
        state->terminal8 = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file((state->terminal8), terminal8, renderer);
        printf("reloaded textures! \n");

    }

    if (key_pressed(keyboard, KB_W)) {
        //create_slice(state, renderer);
    }

    Vec2 offset = get_screen_offset(state->world, screen, side_to_render);


    for (int i = 0; i < entity_count; i++){
        state->game_entities[i].x += state->game_entities[i].velocity.x;
        state->game_entities[i].y += state->game_entities[i].velocity.y;
        int xPos = state->game_entities[i].x;
        int yPos = state->game_entities[i].y;
        if (xPos <= offset.x || xPos >= screen->width - offset.x) {
            state->game_entities[i].velocity.x *= -1;
        }
        if (yPos <= offset.y || yPos >= screen->height - offset.y) {
            state->game_entities[i].velocity.y *= -1;
        }
    }

    SDL_SetRenderDrawColor( renderer, GREY03,  0xFF );
    SDL_RenderClear( renderer );

    int amount = getSliceCount(side_to_render, state->world);
    for (int i = 0; i < amount; i++) {
        s32 x1 = (state->cached->slices[i].bounds.tl.x);
        s32 y1 = (state->cached->slices[i].bounds.tl.y);
        s32 x2 = (state->cached->slices[i].bounds.br.x);
        s32 y2 = (state->cached->slices[i].bounds.br.y);

        if (x2-x1>0 && y2-y1>0) {
            SDL_Rect source = {x1, y1, x2-x1, y2-y1};
            SDL_Rect dest = {x1, y1, x2-x1, y2-y1};

            SDL_RenderCopy(renderer, (state->cached->slices[i].tex.tex), &source, &dest);
        } else {
            //printf("Nothing to draw at slice %d\n",i);
        }
    }

    for (int i = 0; i < entity_count; i++){
        Entity this =  state->game_entities[i];
        SDL_Rect dest = {this.x, this.y, 16, 48};
        SDL_SetRenderDrawColor( renderer, this.red, this.green, this.blue,  0xFF );
        SDL_RenderFillRect(renderer, &dest);
    }

    texture_set_color(state->terminal8, GREY01);
    texture_render_text((state->terminal8), 10, 10, frametime->fps_string, 3, renderer);

    SDL_RenderPresent( renderer );
}


internal void initialize_memory(State *state, Memory* memory, SDL_Renderer* renderer, Screen* screen )
{
    initialize_arena(&state->world_arena,
                     memory->permanent_storage_size - sizeof(State),
                     (u8 *)memory->permanent_storage + sizeof(State));
    state->angle1 = 0;
    state->tex1 = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
    texture_load_from_file( state->tex1, texture1, renderer);
    state->terminal8 = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
    texture_load_from_file( state->terminal8, terminal8, renderer);
    state->render_target = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
    texture_create_blank( state->render_target, 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
    state->zelda = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
    texture_load_from_file( state->zelda, zelda, renderer);

    state->blocks = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
    texture_load_from_file( state->blocks, blocks, renderer);

    state->timer = (Timer *) PUSH_STRUCT(&state->world_arena, Timer);
    timer_init(state->timer);
    timer_start(state->timer);

    state->animation1 = (Animation *) PUSH_STRUCT(&state->world_arena, Animation);
    animation_init(state->animation1);
    animation_add_frame( state->animation1, 0, 200, NULL );
    animation_add_frame( state->animation1, 1, 200, NULL );
    animation_add_frame( state->animation1, 2, 200, NULL );
    animation_add_frame( state->animation1, 3, 200, NULL );
    animation_add_frame( state->animation1, 4, 200, NULL );
    animation_add_frame( state->animation1, 5, 200, NULL );
    animation_add_frame( state->animation1, 6, 200, NULL );
    animation_add_frame( state->animation1, 7, 200, NULL );
    animation_add_frame( state->animation1, 8, 200, NULL );

    state->walking_left = (Sprite *) PUSH_STRUCT(&state->world_arena, Sprite);
    SDL_Rect clip1 = {.x=0, .y=26*5, .w=10*24, .h=26 };
    sprite_init(state->walking_left, state->zelda, clip1, 24, 26);

    state->walking_right = (Sprite *) PUSH_STRUCT(&state->world_arena, Sprite);
    SDL_Rect clip2 = {.x=0, .y=26*7, .w=10*24, .h=26 };
    sprite_init(state->walking_right, state->zelda, clip2, 24, 26);

    state->world = (World *) PUSH_STRUCT(&state->world_arena, World);

    state->world->width  = 50;
    state->world->height = 4;
    state->world->depth  = 50;

    state->world->blocks = (Block*) PUSH_ARRAY(&state->world_arena,
                                               state->world->width * state->world->height * state->world->depth,
                                               Block);

    int slice_count = MAX(state->world->depth, state->world->width);
    printf("slice count:  %d\n",slice_count);
    state->cached = (CachedSlices*)  PUSH_STRUCT(&state->world_arena, CachedSlices);
    state->cached->slices = (TextureWorldSlice*) PUSH_ARRAY(&state->world_arena, slice_count, TextureWorldSlice);
    for (int i = 0; i < slice_count; i++) {
        texture_create_blank( &state->cached->slices[i].tex, screen->width, screen->height, SDL_TEXTUREACCESS_TARGET, renderer);
    }

    state->game_entities = (Entity*) PUSH_ARRAY(&state->world_arena, entity_count, Entity);
    for (int i = 0; i< entity_count; i++){
        state->game_entities[i] = randomEntity();
        Entity e = state->game_entities[i];
        //int randX = randInt(0,state->world->width);
        //int randY = randInt(0,state->world->depth);
        Vec2 pos = get_screen_position_single(state->world, screen, front, e.x, e.y, e.z);

        state->game_entities[i].x = pos.x;
        state->game_entities[i].y = pos.y;
    }

}
