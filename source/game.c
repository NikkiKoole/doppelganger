#include <SDL2/SDL.h>

void game_update_and_render(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xaa, 0xaa, 0xFF );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}
