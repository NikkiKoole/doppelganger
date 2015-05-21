#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/mman.h> //mmap
#include <unistd.h> //usleep
#include "memory.h"
#include "defines.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
char* TITLE = "DoppelGangers";

internal bool init();
internal void closeGame();

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void (*game_update_and_render)(SDL_Renderer *, game_memory *);

game_memory GameMemory = {};

internal void closeGame()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

internal bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) goto SDL_Error;

    window = SDL_CreateWindow( TITLE,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    if (window == NULL) goto SDL_Error;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) goto SDL_Error;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_Image could not initialize");
        goto SDL_Error;
    }

    return true;
  SDL_Error:
    printf("Error in %s, SDl_Error: %s\n", __FUNCTION__, SDL_GetError());
    return false;
}

shared_library libgame = {
    .handle = NULL,
    .name = "./libgame.so",
    .creation_time = 0,
    .size = 0,
    .fn_name = "game_update_and_render"
};

internal void maybe_load_libgame()
{
    // this will load libgame when timestamps differ
    // this happens initially and everytime the libgamne is rewritten.

    stat(libgame.name, &libgame.stats);
    if (libgame.size == 0) {
        libgame.size = (intmax_t)libgame.stats.st_size;
    }
    if ((intmax_t)libgame.stats.st_ctime != libgame.creation_time) {
        if (libgame.stats.st_nlink > 0 && (intmax_t)libgame.stats.st_size == libgame.size){
            usleep(50); //otherwise the file is not yet 'done' being written on linux ;)
            libgame.creation_time = (intmax_t)libgame.stats.st_ctime;
            if (libgame.handle) {
                SDL_UnloadObject(libgame.handle);
            }
            libgame.handle = SDL_LoadObject(libgame.name);
            if (!libgame.handle) {
                printf("couldnt load:%s, error: %s\n",libgame.name, SDL_GetError());
            } else {
                game_update_and_render = (void (*)(SDL_Renderer *, game_memory *))SDL_LoadFunction(libgame.handle, libgame.fn_name);
                if (game_update_and_render == NULL) {
                    printf("couldnt find: %s, error: %s\n",libgame.fn_name, SDL_GetError());
                }
            }
        }
    }
}

void initialize_memory()
{
    void *BaseAddress = (void *) Gigabytes(1);
    GameMemory.PermanentStorageSize = Megabytes(64);
    GameMemory.TransientStorageSize = Gigabytes(1);

    uint64 TotalStorageSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;
    GameMemory.PermanentStorage = mmap(BaseAddress, TotalStorageSize,
                                       PROT_READ | PROT_WRITE,
                                       MAP_ANON | MAP_PRIVATE,
                                       -1, 0);
    GameMemory.TransientStorage = (uint8*)(GameMemory.PermanentStorage) + GameMemory.PermanentStorageSize;
    GameMemory.isInitialized = false;
    GameMemory.wantsTextureRefresh = false;
}

int main()
{
    if( !init() ) {
        printf( "Failed to initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        bool quit = false;
        SDL_Event e;

        initialize_memory();
        maybe_load_libgame();

        while( !quit ) {
            while( SDL_PollEvent( &e ) != 0 ) {
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }  else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_F5:
                        GameMemory.wantsTextureRefresh = true;
                        break;
                    default:
                        break;
                    }
                }
            }
            maybe_load_libgame();
            game_update_and_render(renderer, &GameMemory);
        }
    }
	closeGame();
}
