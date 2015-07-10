#include <SDL2/SDL.h>
#include "memory.h"
#include "state.h"
#include "texture.h"
#include "timer.h"
#include "animation.h"
#include "keyboard.h"
#include "geom.h"
#include "colors.h"
#include <time.h>

char texture1[] = "resources/image.png";
char terminal8[] = "resources/terminal8.png";
char zelda[] = "resources/link.png"; // 24 x 26 px
char blocks[] = "resources/blocks.png"; // 16 x 24 px

internal void initialize_memory(State *state,  Memory* memory, SDL_Renderer* renderer);
internal void create_slice(State *state, SDL_Renderer* renderer  );
void game_update_and_render(Screen *screen, Memory *memory, Keyboard *keyboard, FrameTime *frametime);


// TODO : later on replace this simple stack based crazyness with some transient memory (and make it a linked list or something)
internal void reset_bbox(BBox *v) {
    UNUSED(v);
    printf("getting in here to clean out this BBOX\n");
}


typedef struct BBoxNode
{
    struct BBoxNode *next;
    BBox value;
} BBoxNode;


//void set_structured_values_in_world(World* world);
internal void set_structured_values_in_world(World* world)
{
    // bottom plateau
    for (int x = 0; x < world->width; x++) {
        for (int y = 0 ; y < world->depth; y++) {
            setBlockAt(world, x,y,0,1);
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
    // right wall
    for (int y = 0; y < world->depth; y++) {
        for (int z = 0 ; z < world->height; z++) {
            //setBlockAt(world, world->width-1,y,z,4);
        }
    }
}


extern void game_update_and_render(Screen* screen, Memory* memory, Keyboard* keyboard, FrameTime* frametime)
{
    SDL_Renderer* renderer = screen->renderer;
    ASSERT(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;
    ASSERT(sizeof(TransState) <= memory->transient_storage_size);
    TransState *trans_state = (TransState *)memory->transient_storage;


    if (!memory->is_initialized) {

        printf("what size does the transient have: %d\n",memory->transient_storage_size);
        initialize_memory(state, memory, renderer);

        initialize_arena(&trans_state->scratch_arena,
                         memory->transient_storage_size - sizeof(TransState),
                         (uint8 *)memory->transient_storage + sizeof(TransState));

        /* TempMemory scratch = begin_temporary_memory(&trans_state->scratch_arena); */


        /* BBoxNode *first = (BBoxNode*) PUSH_STRUCT(&trans_state->scratch_arena, BBoxNode); */
        /* BBoxNode *current = first; */
        /* clock_t start = clock() ; */

        /* for (int i = 0; i< 10000; i++){ */
        /*     BBoxNode *new = (BBoxNode*) PUSH_STRUCT(&trans_state->scratch_arena, BBoxNode); */
        /*     new->next = NULL; */
        /*     new->value = bbox(i,i,i,i); */
        /*     current->next = new; */
        /*     current = new; */
        /* } */

        /* char buffer[64]; */
        /* current = first; */
        /* while (current->next != NULL) { */
        /*     bbox_to_buffer(current->value, buffer); */
        /*     current = current->next; */
        /* } */

        /* clock_t end = clock() ; */
        /* double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ; */
        /* printf("doing that linkedlist madness took: %f\n",elapsed_time); */

        //printf("Current scratch: used: %zu\n", scratch.used);
        //printf("Current transstate: used: %zu\n", trans_state->scratch_arena.used);

        //BBox *box1 = (BBox*)PUSH_STRUCT(&trans_state->scratch_arena, BBox);
        //BBox *box2 = (BBox*)PUSH_STRUCT(&trans_state->scratch_arena, BBox);
        //*box1 = bbox(1,1,100,100);

        //printf("Current transstate: used: %zu\n", trans_state->scratch_arena.used);
        //end_temporary_memory(scratch, &trans_state->scratch_arena);
        //printf("Current transstate: used: %zu\n", trans_state->scratch_arena.used);

        //scratch = begin_temporary_memory(&trans_state->scratch_arena);
        //box2 = (BBox*)PUSH_STRUCT(&trans_state->scratch_arena, BBox);
        //char buffer[64];
        //bbox_to_buffer(*box2, buffer);
        //bbox_to_buffer(*box1, buffer);
        //printf("addres: %p %s\n", &box2, buffer);

        memory->is_initialized = true;
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

    SDL_SetRenderDrawColor( renderer, GREY03,  0xFF );
    SDL_RenderClear( renderer );

    resetBlocks(state->world);
    set_structured_values_in_world(state->world);

    draw_3d_space(state->world, front, renderer, screen, state->blocks, trans_state);

    texture_set_color(state->terminal8, PINK);
    texture_render_text((state->terminal8), 10, 10, frametime->fps_string, 3, renderer);
    SDL_RenderPresent( renderer );
}



internal void create_slice(State *state, SDL_Renderer* renderer  ) {

    clock_t start = clock() ;

    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
    SDL_RenderClear( renderer );
    texture_set_as_rendertarget(&state->world_slices[0], renderer);
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( renderer );
    int startX = 30;
    int startY = 400;

    for (int height = 0; height < 32; height++) {
        for (int depth = 0; depth< 32; depth++) {
            texture_set_color((state->blocks), 0xFF - height*4, 0xFF - depth*4, 0xFF);
            for (int width = 0; width < 32; width++) {
                int index = rand() % 6;
                SDL_Rect source = {.x=index*16, .y=0, .w=16, .h=24};
                SDL_Rect dest = {.x=startX+width*16, .y=startY+(depth*8)-(height*16), .w=16, .h=24};

                if (rand() % 100 < 90) {
                    texture_render_part(state->blocks, &source, &dest, renderer);
                }
            }
        }
    }
    SDL_SetRenderTarget( renderer, NULL );

    clock_t end = clock() ;
    double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;
    printf("create slice took:  %f\n", elapsed_time);
}


internal void initialize_memory(State *state, Memory* memory, SDL_Renderer* renderer)
{
            initialize_arena(&state->world_arena,
                        memory->permanent_storage_size - sizeof(State),
                        (uint8 *)memory->permanent_storage + sizeof(State));
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
        state->world->width = 60;
        state->world->height = 50;
        state->world->depth = 20;
        state->world->blocks = (Block*) PUSH_ARRAY(&state->world_arena,
                                                   state->world->width * state->world->height * state->world->depth,
                                                   Block);


        //printf("world: %zu\n", sizeof(World));
        //printf("world blocks: %d\n", state->world->width*state->world->height*state->world->depth);

        //printf("single texture: %zu\n", sizeof(Texture));
        //printf("Arena size before pushing Array: %zu \n", state->world_arena.used);
        //state->world_slices = (Texture*) PUSH_ARRAY(&state->world_arena, MAX(state->world->depth, state->world->width), Texture);
        //printf("Arena size after pushing Array: %zu \n", state->world_arena.used);

        //texture_create_blank( &state->world_slices[0], 1024*2, 768*2, SDL_TEXTUREACCESS_TARGET, renderer);
        //texture_create_blank( &state->world_slices[1], 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
        //texture_create_blank( &state->world_slices[2], 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
        //printf("the third blank texture, living in world_slices:{ w:%d, h:%d } \n",state->world_slices[2].width, state->world_slices[2].height);
        //create_slice(state, renderer);

}



internal void oldstuff(void)
{
#if 0
    //texture_set_color((state->tex1), 0xFF, 0x00, 0xFF);
    //exture_set_alpha((state->tex1), 180);
    //texture_set_alpha((state->tex1), 180);
    //texture_render((&state->world_slices[2]), 100, 100, renderer);
    //texture_set_alpha((state->world_slices[0]), 10);

    for (int i = 0; i< 10 ; i++) {
        texture_render((&state->world_slices[0]), 0, 0, renderer);
    }


#if 0
    state->angle1+= .00125;
    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
    SDL_RenderClear( renderer );
    texture_set_blend_mode((state->render_target), SDL_BLENDMODE_BLEND);
    texture_set_as_rendertarget(state->render_target, renderer);
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( renderer );
s
    texture_set_color((state->tex1), 0xFF, 0xFF, 0xFF);
    texture_set_alpha((state->tex1), 180);
    texture_render((state->tex1), 100, 100, renderer);
    texture_render((state->tex1), 100, 100, renderer);

    SDL_SetRenderTarget( renderer, NULL );
    texture_render((state->render_target), 100, 100, renderer);

    texture_set_color((state->tex1), 0xFF, 0xFF, 0xFF);
    texture_set_alpha((state->tex1), 180);
    texture_render((state->tex1), 100, 100, renderer);
    texture_render((state->tex1), 100, 100, renderer);

    texture_set_color((state->tex1), 0xFF, 0xAA, 0x00);
    texture_set_alpha((state->tex1), 250);
    texture_render_ex((state->tex1), 200, 100, NULL, state->angle1 , NULL, SDL_FLIP_NONE,  renderer);

    for (int i = 0; i < 3; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 100, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    for (int i = 0; i < 3; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 400, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    texture_set_color((state->zelda), 0xFF, 0xFF, 0xFF);
    texture_set_alpha((state->zelda), 255);
    texture_render((state->zelda), 100, 100, renderer);


    texture_set_color((state->terminal8), 0xff, 0xaa, 0xff);
    texture_render_text((state->terminal8), 10, 100, "Here's some text\nLorem ipsum dolor sit amet, ea vix modo\ntantas prodesset, nec ne veri salutandi\nhonestatis, ad nam omittam adipiscing.\nAt modus verterem abhorreant duo.\nNe lorem imperdiet qui.\nAt nam exerci civibus scribentur, \nea per nisl inimicus\nevertitur, ut vocent similique ius.\nSonet deserunt no mea, quo id\nscriptorem signiferumque,\nmel ad unum essent elaboraret.\n", 3, renderer);
#endif

#if 1
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            SDL_Rect source = sprite_get_current_frame(state->walking_left, state->animation1);
            if ((i+j) % 2 == 0) {
                source = sprite_get_current_frame(state->walking_right, state->animation1);
            }
            SDL_Rect dest = {.x=i*30, .y=j*30, .w=24, .h=26};
            texture_render_part(state->zelda, &source, &dest, renderer);
        }
    }
    sprite_update_elapsed_time(state->walking_left, frametime->duration);
    sprite_update_elapsed_time(state->walking_right, frametime->duration);
#endif

    texture_render_text((state->terminal8), 10, 10, frametime->fps_string, 1, renderer);
#endif
}
