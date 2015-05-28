#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "memory.h"
#include "texture.h"
#include "timer.h"

char texture1[] = "resources/image.png";
char terminal8[] = "resources/terminal8.png";

char font1[] = "resources/font.ttf";
#define push_struct(arena, type) (type *)push_size_(arena, sizeof(type))
#define push_array(arena, count, type) (type *)push_size_(arena, (count)*sizeof(type))

void* push_size_(Memory_Arena *arena, memory_index size){
    Assert(arena->used + size <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    return result;
}

void initialize_arena(Memory_Arena *arena, memory_index size, uint8 *base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

float total_frames = 0;
void game_update_and_render(Screen* screen, Memory *memory)
{
    SDL_Renderer* renderer = screen->renderer;
    Assert(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;

    if (!memory->is_initialized) {
        initialize_arena(&state->world_arena,
                        memory->permanent_storage_size - sizeof(State),
                        (uint8 *)memory->permanent_storage + sizeof(State));
        state->angle1 = 0;
        state->tex1 = (Texture*) push_struct(&state->world_arena, Texture);
        texture_load_from_file((state->tex1), texture1, renderer);
        state->terminal8 = (Texture*) push_struct(&state->world_arena, Texture);
        texture_load_from_file((state->terminal8), terminal8, renderer);
        state->render_target = (Texture*) push_struct(&state->world_arena, Texture);
        texture_create_blank(state->render_target, 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
        state->timer = (Timer*) push_struct(&state->world_arena, Timer);
        timer_init(state->timer);
        //texture_create_blank(state->render_target, 1024, 768, SDL_TEXTUREACCESS_TARGET, renderer);
        timer_start(state->timer);
        memory->is_initialized = true;
    }

    if (memory->wants_texture_refresh) {
        Assert(state->tex1);
        texture_load_from_file((state->tex1), texture1, renderer);
        state->terminal8 = (Texture*) push_struct(&state->world_arena, Texture);
        texture_load_from_file((state->terminal8), terminal8, renderer);
        memory->wants_texture_refresh = false;
    }
    
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

    for (int i = 0; i < 180; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 100, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    for (int i = 0; i < 180; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 400, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    texture_set_color((state->terminal8), 0x00, 0xaa, 0xff);
    texture_render_text((state->terminal8), 10, 100, "Here's some text\nLorem ipsum dolor sit amet, ea vix modo\ntantas prodesset, nec ne veri salutandi\nhonestatis, ad nam omittam adipiscing.\nAt modus verterem abhorreant duo.\nNe lorem imperdiet qui.\nAt nam exerci civibus scribentur, \nea per nisl inimicus\nevertitur, ut vocent similique ius.\nSonet deserunt no mea, quo id\nscriptorem signiferumque,\nmel ad unum essent elaboraret.\n", 3, renderer);
    char buf[sizeof(int)*3+2];
    snprintf(buf, sizeof buf, "%f", total_frames/(timer_get_ticks(state->timer)/1000.f));
    texture_render_text((state->terminal8), 10, 10, buf, 3, renderer);
    SDL_RenderPresent( renderer );
    ++total_frames;
}
