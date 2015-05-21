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
        GameState->tex1 = (texture*) PushStruct(&GameState->WorldArena, texture);
        texture_load_from_file((GameState->tex1), texture1, Renderer);
        Memory->isInitialized = true;
    }

    if (Memory->wantsTextureRefresh) {
        Assert(GameState->tex1);
        texture_load_from_file((GameState->tex1), texture1, Renderer);
        Memory->wantsTextureRefresh = false;
    }

    SDL_SetRenderDrawColor( Renderer, 0x00, 0xAA, 0x99, 0xFF );
    SDL_RenderClear( Renderer );

    for (int i = 0; i<100; i+=1) {
        texture_set_color((GameState->tex1), i*2, 0xFF-i, i*7);
        texture_set_alpha((GameState->tex1), i);
        texture_render((GameState->tex1), 100+i, 100+i*2, Renderer);
        texture_set_alpha((GameState->tex1), 255-i*3);
        texture_set_color((GameState->tex1), 0xFF*10*i, 0xFF*10*i, 0xFF);
        texture_render((GameState->tex1), 10, 0+i, Renderer);
    }
    SDL_RenderPresent( Renderer );

}
