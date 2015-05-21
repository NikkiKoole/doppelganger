#include <SDL2/SDL.h>
#include "memory.h"
#include "texture.h"

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


void game_update_and_render(SDL_Renderer *Renderer, game_memory *Memory)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;

    if (!Memory->isInitialized) {
        InitializeArena(&GameState->WorldArena,
                        Memory->PermanentStorageSize - sizeof(game_state),
                        (uint8 *)Memory->PermanentStorage + sizeof(game_state));
        GameState->angle1 = 0;
        GameState->tex1 = (texture*) PushStruct(&GameState->WorldArena, texture);
        texture_load_from_file((GameState->tex1), texture1, Renderer);
        
        Memory->isInitialized = true;
    }

    if (Memory->wantsTextureRefresh) {
        Assert(GameState->tex1);
        texture_load_from_file((GameState->tex1), texture1, Renderer);
        Memory->wantsTextureRefresh = false;
    }

    GameState->angle1+= .059;

    SDL_SetRenderDrawColor( Renderer, 0x00, 0x11, 0x99, 0xFF );
    SDL_RenderClear( Renderer );
    texture_set_color((GameState->tex1), 0xFF, 0xF, 0xFF);
    texture_set_alpha((GameState->tex1), 180);
    texture_render((GameState->tex1), 100, 100, Renderer);
    
    texture_set_color((GameState->tex1), 0x00, 0xAA, 0x00);
    texture_set_alpha((GameState->tex1), 250);
    texture_render_ex((GameState->tex1), 400, 100, NULL, GameState->angle1 , NULL, SDL_FLIP_NONE,  Renderer);
    SDL_RenderPresent( Renderer );
}

