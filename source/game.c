#include <SDL2/SDL.h>
#include "tile.h"
#include "memory.h"

void game_update_and_render(SDL_Renderer *renderer, Memory *memory)
{
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
    //tile_stuff();
    //printf("The adress of the *Memory you fed me is %p\n", &memory);
}
