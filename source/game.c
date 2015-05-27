#include <SDL2/SDL.h>
#include "memory.h"
#include "texture.h"
#include "internals.h"

char texture1[] = "resources/image.png";

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, count, type) (type *)PushSize_(Arena, (count)*sizeof(type))
void* PushSize_(memory_arena *Arena, memory_index Size){
    Assert(Arena->Used + Size <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    return Result;
}

void InitializeArena(memory_arena *Arena, memory_index Size, uint8 *Base)
{
    Arena->Size = Size;
    Arena->Base = Base;
    Arena->Used = 0;
}


void game_update_and_render(screen_container* screen, game_memory *Memory)
{
    SDL_Renderer* Renderer = screen->renderer;
    SDL_Window* Window = screen->window;
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;

    if (!Memory->isInitialized) {
        InitializeArena(&GameState->WorldArena,
                        Memory->PermanentStorageSize - sizeof(game_state),
                        (uint8 *)Memory->PermanentStorage + sizeof(game_state));
        GameState->angle1 = 0;
        GameState->tex1 = (texture*) PushStruct(&GameState->WorldArena, texture);
        texture_load_from_file((GameState->tex1), texture1, Renderer, Window);
        
        Memory->isInitialized = true;
    }

    if (Memory->wantsTextureRefresh) {
        Assert(GameState->tex1);
        texture_load_from_file((GameState->tex1), texture1, Renderer, Window);
        Memory->wantsTextureRefresh = false;
    }

    GameState->angle1+= .125;

    SDL_SetRenderDrawColor( Renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderClear( Renderer );

    texture_set_color((GameState->tex1), 0xFF, 0xFF, 0xFF);
    texture_set_alpha((GameState->tex1), 180);
    texture_render((GameState->tex1), 100, 100, Renderer);
    
    texture_set_color((GameState->tex1), 0xFF, 0xAA, 0x00);
    texture_set_alpha((GameState->tex1), 250);
    texture_render_ex((GameState->tex1), 200, 100, NULL, GameState->angle1 , NULL, SDL_FLIP_NONE,  Renderer);
    
    for (int i = 0; i < 18; i+=1) {
        texture_set_color((GameState->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((GameState->tex1), 150-i*10);
        texture_render_ex((GameState->tex1), 5.0*i, 100, NULL, 360 - GameState->angle1*i , NULL, SDL_FLIP_NONE,  Renderer);
    }
    for (int i = 0; i < 18; i+=1) {
        texture_set_color((GameState->tex1), 0xFF, 0xFF, 0x00);
        texture_set_alpha((GameState->tex1), 150-i*10);
        texture_render_ex((GameState->tex1), 5.0*i, 400, NULL, 360 - GameState->angle1*i , NULL, SDL_FLIP_NONE,  Renderer);
    }
    SDL_RenderPresent( Renderer );
}

