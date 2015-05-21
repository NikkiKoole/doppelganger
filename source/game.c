#include <SDL2/SDL.h>
#include "memory.h"
#include "texture.h"


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
        init_texture(GameState->tex1);
        texture_load_from_file((GameState->tex1), "resources/image.png", Renderer);
        Memory->isInitialized = true;
    }

    SDL_SetRenderDrawColor( Renderer, 0x00, 0x00, 0xFF, 0xAA );
    SDL_RenderClear( Renderer );
    texture_render((GameState->tex1), 210, 130, Renderer);
    texture_render((GameState->tex1), 10, 30, Renderer);
    SDL_RenderPresent( Renderer );

}
