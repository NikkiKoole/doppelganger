#ifndef MEMORY_H
#define MEMORY_H

#include <sys/stat.h>   //struct stat
#include <sys/types.h>  //struct stat

#include "texture.h"
#include "defines.h"
#include "timer.h"
#include "animation.h"
#include "sprite.h"
#include "keyboard.h"

typedef struct Screen
{
    SDL_Renderer* renderer;
    SDL_Window* window;
} Screen;

typedef struct Shared_Library
{
    void* handle;
    const char* name;
    intmax_t creation_time;
    uint32 id;
    int32 size;
    const char* fn_name;
    struct stat stats;
} Shared_Library;

typedef struct FrameTime
{
    int duration;
    char fps_string[64];

} FrameTime;


typedef struct Memory
{
    bool is_initialized;

	uint32 permanent_storage_size;
    void *permanent_storage;

    uint32 transient_storage_size;
	void *transient_storage;
} Memory;

typedef struct Memory_Arena
{
    memory_index size;
    uint8 *base;
    memory_index used;
} Memory_Arena;


// TODO : expand this babe to greatness!
typedef struct State
{
    Memory_Arena world_arena;
    double angle1;
    Texture* tex1;
    Texture* terminal8;
    Texture* render_target;
    Texture* zelda;
    Timer* timer;
    Animation* animation1;
    Sprite* link1;
} State;

// this is the function thats being called from main, offered up by the gamelib.so file.
void game_update_and_render(Screen* screen, Memory* memory, Keyboard* keyboard, FrameTime* frametime);

#endif
