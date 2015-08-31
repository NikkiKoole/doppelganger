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

Side side_to_render  = right;

#define entity_count 300

internal void initialize_memory(State *state,  Memory* memory, SDL_Renderer* renderer, Screen* screen);
internal void create_slice(State *state, SDL_Renderer* renderer  );
void game_update_and_render(Screen *screen, Memory *memory, Keyboard *keyboard, FrameTime *frametime);

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
            setBlockAt(world, 0,y,z,3);
        }
    }
    // front left column
    for (int z = 0 ; z < world->height; z++) {
        setBlockAt(world, 0,world->depth-1,z,3);
    }

    // right wall
    for (int y = 0; y < world->depth; y++) {
        for (int z = 0 ; z < world->height; z++) {
            setBlockAt(world, world->width-1,y,z,4);
        }
    }
}

internal int getSliceCount(Side side, World *world) {
    if (side == left || side == right) {
        return world->width;
    }
    return world->depth;
}

internal void create_world(State *state, SDL_Renderer* renderer, Screen* screen,  TransState *trans_state) {
    resetBlocks(state->world);
    set_structured_values_in_world(state->world);
    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
    SDL_RenderClear( renderer );

    for (int i = 0; i < getSliceCount(side_to_render, state->world); i++) {
        //texture_set_as_rendertarget(&state->cached->slices[i].tex, renderer);
        //texture_set_blend_mode(&state->cached->slices[i].tex, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget( renderer, state->cached->slices[i].tex.SDLtex);
        SDL_SetTextureBlendMode(state->cached->slices[i].tex.SDLtex, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( renderer );
    }

    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( renderer );

    draw_3d_space(state->world, side_to_render, renderer, screen, state->blocks, trans_state, state->cached);
    SDL_SetRenderTarget( renderer, NULL );

}


extern void game_update_and_render(Screen* screen, Memory* memory, Keyboard* keyboard, FrameTime* frametime)
{
    SDL_Renderer* renderer = screen->renderer;
    ASSERT(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;
    ASSERT(sizeof(TransState) <= memory->transient_storage_size);
    TransState *trans_state = (TransState *)memory->transient_storage;
    Vec2 offset;
    int sliceCount;
    // todo (when live editing) make side change rerender.

    if (!memory->is_initialized) {
        printf("initialize memory\n");
        initialize_memory(state, memory, renderer, screen);
        initialize_arena(&trans_state->scratch_arena,
                         memory->transient_storage_size - sizeof(TransState),
                         (u8 *)memory->transient_storage + sizeof(TransState));

        memory->is_initialized = true;
    }

    Side last_side = (Side)state->last_side;
    if (side_to_render != last_side) {
        printf("changed sides\n");
        state->last_side = side_to_render;
        create_world(state, renderer, screen, trans_state);
    }


    if (key_pressed(keyboard,KB_F5)){
        ASSERT(state->tex1);
        texture_load_from_file((state->tex1), texture1, renderer);
        state->terminal8 = (Texture *) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file((state->terminal8), terminal8, renderer);
        printf("reloaded textures! \n");
        create_world(state, renderer, screen, trans_state);
    }

    //I will always update the world as a front view.
    //Then when rendering the wolrd or entites take note of the current side.
    offset = get_screen_offset(state->world, screen, side_to_render);
    float timestep = 1000.0/frametime->duration;
    int entityWidth = 12;
    int entityHeight = 12;
    for (int i = 0; i < entity_count; i++){

        state->game_entities[i].x += state->game_entities[i].velocity.x / timestep;
        state->game_entities[i].y += state->game_entities[i].velocity.y / timestep;

        int xPos = state->game_entities[i].x;
        int yPos = state->game_entities[i].y;
        BBox b = bbox(xPos-entityWidth/2, yPos-entityHeight/2, xPos+entityWidth/2, yPos+entityHeight/2);
        if (b.tl.x <= 0 || b.br.x >= state->world->width * 16) {
            state->game_entities[i].velocity.x *= -1;
        }
        if (b.tl.y <= 0 || b.br.y   >= state->world->depth * 16 ) {
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

            SDL_RenderCopy(renderer, (state->cached->slices[i].tex.SDLtex), &source, &dest);
        } else {
            //printf("Nothing to draw at slice %d\n",i);
        }
    }

    offset = get_screen_offset(state->world, screen, side_to_render);

    for (int i = 0; i < entity_count; i++){
        Entity this =  state->game_entities[i];
        Vec2 translated = get_translated_single2(state->world, side_to_render, this.x, this.y, this.z);
        SDL_Rect dest = {translated.x+offset.x-entityWidth/2, translated.y+offset.y-entityHeight/2, entityWidth, entityHeight};
        SDL_SetRenderDrawColor( renderer, this.red, this.green, this.blue,  0xFF );
        SDL_RenderFillRect(renderer, &dest);
    }
    SDL_SetTextureColorMod(state->terminal8->SDLtex, GREY01);
    //texture_set_color(state->terminal8, GREY01);
    texture_render_text((state->terminal8), 10, 10, frametime->fps_string, 3, renderer);

    SDL_RenderPresent( renderer );
}



internal void initialize_memory(State *state, Memory* memory, SDL_Renderer* renderer, Screen* screen )
{
    initialize_arena(&state->world_arena,
                     memory->permanent_storage_size - sizeof(State),
                     (u8 *)memory->permanent_storage + sizeof(State));
    state->angle1 = 0;

    state->last_side = (Side) PUSH_STRUCT(&state->world_arena, Side);
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
    state->world->width  = 20;
    state->world->height = 1;
    state->world->depth  = 20;
    state->world->blocks = (Block*) PUSH_ARRAY(&state->world_arena,
                                               state->world->width * state->world->height * state->world->depth,
                                               Block);

    int slice_count = MAX(state->world->depth, state->world->width);
    state->cached = (CachedSlices*)  PUSH_STRUCT(&state->world_arena, CachedSlices);
    state->cached->slices = (TextureWorldSlice*) PUSH_ARRAY(&state->world_arena, slice_count, TextureWorldSlice);
    for (int i = 0; i < slice_count; i++) {
        texture_create_blank( &state->cached->slices[i].tex, screen->width, screen->height, SDL_TEXTUREACCESS_TARGET, renderer);
    }

    state->game_entities = (Entity*) PUSH_ARRAY(&state->world_arena, entity_count, Entity);
    for (int i = 0; i< entity_count; i++){
        state->game_entities[i] = randomEntity();
        Entity *e = &state->game_entities[i];
        e->x = (state->world->width)*8;
        e->y = (state->world->depth)*8;
    }

}
