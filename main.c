#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
struct stat libStat;

char* myFunctionName = "game_update_and_render";
void (*game_update_and_render)(SDL_Renderer *);
long long creationTime;

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
	if (myHandle){
        SDL_UnloadObject(myHandle);
    }
    myHandle = SDL_LoadObject(libraryName);
    if (!myHandle){
        printf("couldnt load:%s, error: %s\n",libraryName, SDL_GetError());
    }
    game_update_and_render = (void (*)(SDL_Renderer *))SDL_LoadFunction(myHandle, myFunctionName);
    stat(libraryName, &libStat);

    // TODO instead of this long long madness try to handle the timestamp more safely
    creationTime = (long long)libStat.st_mtime;

    if (game_update_and_render != NULL) {
        game_update_and_render(renderer);
    } else {
        printf("couldnt run: %s, error: %s\n",myFunctionName, SDL_GetError());
    }

}


int main(){
    if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        bool quit = false;
        SDL_Event e;
        loadEntryPointFunctionAndRun();

        while( !quit )
		{
			while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }  else if (e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                    }
                }

                stat(libraryName, &libStat);
                if ((long long)libStat.st_mtime != creationTime){
                    loadEntryPointFunctionAndRun();
                }
            }
        }
	}
	closeGame();
}
