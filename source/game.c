#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "memory.h"
#include "texture.h"

char texture1[] = "resources/image.png";

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


void game_update_and_render(Screen* screen, Memory *memory)
{
    //printf("%d", sizeof(TTF_Font));

    SDL_Renderer* renderer = screen->renderer;
    //SDL_Window* window = screen->window;
    Assert(sizeof(State) <= memory->permanent_storage_size);
    State *state = (State *)memory->permanent_storage;

    if (!memory->is_initialized) {
        initialize_arena(&state->world_arena,
                        memory->permanent_storage_size - sizeof(State),
                        (uint8 *)memory->permanent_storage + sizeof(State));
        state->angle1 = 0;
        state->tex1 = (Texture*) push_struct(&state->world_arena, Texture);
        texture_load_from_file((state->tex1), texture1, renderer);
        memory->is_initialized = true;
    }

    if (memory->wants_texture_refresh) {
        Assert(state->tex1);
        texture_load_from_file((state->tex1), texture1, renderer);
        memory->wants_texture_refresh = false;
    }

    state->angle1+= .125;

    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderClear( renderer );

    texture_set_color((state->tex1), 0xFF, 0xFF, 0xFF);
    texture_set_alpha((state->tex1), 180);
    texture_render((state->tex1), 100, 100, renderer);

    texture_set_color((state->tex1), 0xFF, 0xAA, 0x00);
    texture_set_alpha((state->tex1), 250);
    texture_render_ex((state->tex1), 200, 100, NULL, state->angle1 , NULL, SDL_FLIP_NONE,  renderer);

    for (int i = 0; i < 18; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 100, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    for (int i = 0; i < 18; i+=1) {
        texture_set_color((state->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((state->tex1), 150-i*10);
        texture_render_ex((state->tex1), 5.0*i, 400, NULL, 360 - state->angle1*i , NULL, SDL_FLIP_NONE,  renderer);
    }
    SDL_RenderPresent( renderer );
}
