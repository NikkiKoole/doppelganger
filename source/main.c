#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/mman.h> //mmap
#include <unistd.h> //usleep
#include "memory.h"
#include "defines.h"
#include "keyboard.h"

const int32 SCREEN_WIDTH = 1280;
const int32 SCREEN_HEIGHT = 800;
const char* TITLE = "DoppelGangers";

internal bool init(void);
internal void close_game(void);
internal void maybe_load_libgame(void);
internal void initialize_memory(void);

Screen global_screen;
Screen* screen = &global_screen;
Keyboard global_keyboard;
Keyboard* keyboard = &global_keyboard;
FrameTime global_frame_time;
FrameTime* frame_time = &global_frame_time;

Memory memory;

void (*func)(Screen *, Memory *, Keyboard *, FrameTime*);

internal void close_game(void)
{
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    screen->window = NULL;
    screen->renderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

internal bool init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) goto SDL_Error;

    SDL_Window* window = SDL_CreateWindow( TITLE,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    if (window == NULL) goto SDL_Error;
    screen->window = window;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) goto SDL_Error;
    screen->renderer = renderer;

    SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0x00, 0xFF, 0xFF);

    int32 img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_Image could not initialize");
        goto SDL_Error;
    }

    return true;
  SDL_Error:
    printf("Error in %s, SDl_Error: %s\n", __FUNCTION__, SDL_GetError());
    return false;

}

Shared_Library libgame = {
    .handle = NULL,
    .name = "./libgame.so",
    .creation_time = 0,
    .size = 0,
    .fn_name = "game_update_and_render"
};


internal void stub(Screen* s, Memory* m, Keyboard* k, FrameTime* f)
{
    UNUSED(s);  UNUSED(m);  UNUSED(k);  UNUSED(f);
    usleep(10000);
}

internal void maybe_load_libgame(void)
{
    stat(libgame.name, &libgame.stats);
    if (libgame.stats.st_ino != libgame.id){
        if ((intmax_t)libgame.stats.st_size > 0 && libgame.stats.st_nlink > 0) {
            libgame.id = libgame.stats.st_ino;
            if (libgame.handle) {
                SDL_UnloadObject(libgame.handle);
            }
            libgame.handle = SDL_LoadObject(libgame.name);
            if (!libgame.handle) {
                libgame.handle = NULL;
                libgame.id = 0;
                printf("couldnt load:%s, error: %s\n", libgame.name, SDL_GetError());
                func = stub;
            } else {
                func = (void (*)(Screen *, Memory *, Keyboard *, FrameTime *)) SDL_LoadFunction(libgame.handle, libgame.fn_name);
                if (func == NULL) {
                    printf("couldnt find: %s, error: %s\n",libgame.fn_name, SDL_GetError());
                } else {
                    printf("succes loading libgame\n");
                }
            }
        }
    }
}

internal void initialize_memory(void)
{
    void *base_address = (void *) gigabytes(1);
    memory.permanent_storage_size = megabytes(64);
    memory.transient_storage_size = gigabytes(1);

    uint64 total_storage_size = memory.permanent_storage_size + memory.transient_storage_size;
    memory.permanent_storage = mmap(base_address, total_storage_size,
                                    PROT_READ | PROT_WRITE,
                                    MAP_ANON | MAP_PRIVATE,
                                    -1, 0);
    memory.transient_storage = (uint8*)(memory.permanent_storage) + memory.permanent_storage_size;
    memory.is_initialized = false;
}

#define FRAMERATE 1000/30

int main(void)
{
    uint32 now;
    uint32 start;
    srand(0);
    if( !init() ) {
        printf( "Failed to initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        bool quit = false;
        SDL_Event e;

        initialize_memory();
        maybe_load_libgame();
        start = SDL_GetTicks();

        while( !quit ) {
            while( SDL_PollEvent( &e ) != 0 ) {
                keyboard->keys = SDL_GetKeyboardState( NULL );
                if( e.type == SDL_QUIT || key_pressed(keyboard,KB_ESC) ){quit = true;}
            }

            maybe_load_libgame();
            now = SDL_GetTicks();

            if ( now - start < FRAMERATE ) SDL_Delay( FRAMERATE - ( now - start ) );
            now = SDL_GetTicks();

            frame_time->duration = now - start;
            int32 fps = 1000/frame_time->duration;
            snprintf(frame_time->fps_string, sizeof frame_time->fps_string, "%d %s", fps, "FPS");

            func(screen, &memory, keyboard, frame_time);
            start = now;
        }
    }
	close_game();
    return 1;
}
