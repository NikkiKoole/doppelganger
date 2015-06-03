#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "memory.h"
#include "texture.h"
#include "timer.h"
#include "animation.h"
#include "keyboard.h"
//#include "sprite.h"
#include <time.h>

char texture1[] = "resources/image.png";
char terminal8[] = "resources/terminal8.png";
char zelda[] = "resources/link.png"; //24x26 pixels

#define PUSH_STRUCT(arena, type) (type *)push_size_(arena, sizeof(type))
//#define push_array(arena, count, type) (type *)push_size_(arena, (count)*sizeof(type))

internal void* push_size_(Memory_Arena *arena, memory_index size){
    ASSERT(arena->used + size <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    return result;
}

internal void initialize_arena(Memory_Arena *arena, memory_index size, uint8 *base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

extern void game_update_and_render(Screen* screen, Memory* memory, Keyboard* keyboard, FrameTime* frametime)
{
    SDL_Renderer* renderer = screen->renderer;
    ASSERT(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;

    if (!memory->is_initialized) {
        initialize_arena(&state->world_arena,
                        memory->permanent_storage_size - sizeof(State),
                        (uint8 *)memory->permanent_storage + sizeof(State));
        state->angle1 = 0;
        state->tex1 = (Texture*) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file( state->tex1, texture1, renderer);
        state->terminal8 = (Texture*) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file( state->terminal8, terminal8, renderer);
        state->render_target = (Texture*) PUSH_STRUCT(&state->world_arena, Texture);
        texture_create_blank( state->render_target, 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
        state->zelda = (Texture*) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file( state->zelda, zelda, renderer);



        state->timer = (Timer*) PUSH_STRUCT(&state->world_arena, Timer);
        timer_init(state->timer);
        timer_start(state->timer);
        state->animation1 = (Animation*) PUSH_STRUCT(&state->world_arena, Animation);
        animation_init(state->animation1);
        animation_add_frame( state->animation1, 0, 200, NULL );
        animation_add_frame( state->animation1, 1, 200, NULL );
        animation_add_frame( state->animation1, 2, 200, NULL );
        animation_add_frame( state->animation1, 3, 200, NULL );
        state->link1 = (Sprite*) PUSH_STRUCT(&state->world_arena, Sprite);
        SDL_Rect clip = {.x=0, .y=26*5, .w=10*24, .h=26 };
        sprite_init(state->link1, state->zelda, clip, 24, 26);

        //sprite_play_animation(state->link1, state->animation1);
        memory->is_initialized = true;
    }

    if (key_pressed(keyboard,KB_F5)){
        ASSERT(state->tex1);
        texture_load_from_file((state->tex1), texture1, renderer);
        state->terminal8 = (Texture*) PUSH_STRUCT(&state->world_arena, Texture);
        texture_load_from_file((state->terminal8), terminal8, renderer);
        printf("reloaded textures! \n");
    }

    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
    SDL_RenderClear( renderer );
    SDL_Rect source = {.x=0, .y=0, .w=24, .h=26};
    SDL_Rect dest = {.x=100, .y=100, .w=24, .h=26};
    texture_render_part(state->zelda, &source, &dest, renderer);


    state->link1->elapsed_time += frametime->duration;
    printf("now the animations elapsed is at %d Millisceonds (duration: %d)\n ", state->link1->elapsed_time, frametime->duration);
    //sprite_render_current_frame()


#if 0
    state->angle1+= .00125;
    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0xff, 0xFF );
    SDL_RenderClear( renderer );
    texture_set_blend_mode((state->render_target), SDL_BLENDMODE_BLEND);
    texture_set_as_rendertarget(state->render_target, renderer);
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( renderer );

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

    texture_render_text((state->terminal8), 10, 10, frametime->fps_string, 1, renderer);

    SDL_RenderPresent( renderer );
}
