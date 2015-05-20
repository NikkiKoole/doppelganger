#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#include <sys/inotify.h>

#include "memory.h"
#define internal static

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
char* TITLE = "DoppelGangers";

internal bool init();
internal void closeGame();

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void* myHandle = NULL;
char* libraryName = "./libgame.so";
int librarySize;
struct stat libStat;

char* myFunctionName = "game_update_and_render";
void (*game_update_and_render)(SDL_Renderer *, Memory *);
long long creationTime;

Memory mem = {.a = 100};

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


void loadEntryPointFunctionAndRun()
{
    printf("loading %s.\n",libraryName);
	if (myHandle) {
        SDL_UnloadObject(myHandle);
    }
    myHandle = SDL_LoadObject(libraryName);
    if (!myHandle) {
        printf("couldnt load:%s, error: %s\n",libraryName, SDL_GetError());
    }

    game_update_and_render = (void (*)(SDL_Renderer *, Memory *))SDL_LoadFunction(myHandle, myFunctionName);
    if (game_update_and_render != NULL) {
        game_update_and_render(renderer, &mem);
    } else {
        printf("couldnt run: %s, error: %s\n",myFunctionName, SDL_GetError());
    }
}




int main()
{
    if( !init() ) {
        printf( "Failed to initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        bool quit = false;
        SDL_Event e;
        stat(libraryName, &libStat);
        librarySize = (intmax_t)libStat.st_size;
        creationTime = (long long)libStat.st_ctime;
        loadEntryPointFunctionAndRun();

        while( !quit ) {
            while( SDL_PollEvent( &e ) != 0 ) {
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }  else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_r:
                        printf("doing stuff here too you know");
                        system("make game");
                    default:
                        break;
                    }
                }
            }

            stat(libraryName, &libStat);
            if ((long long)libStat.st_ctime != creationTime) {
                    stat(libraryName, &libStat);
                    if (libStat.st_nlink > 0 && (intmax_t)libStat.st_size == librarySize){
                        usleep(50); //otherwise the file is not yet 'done' being written on linux ;)
                        creationTime = (long long)libStat.st_ctime;
                        loadEntryPointFunctionAndRun();
                    }
            }
        }
    }
	closeGame();
}
