#ifndef MEMORY_H
#define MEMORY_H

#include <sys/stat.h>   //struct stat
#include <sys/types.h>  //struct stat

#include "texture.h"
#include "defines.h"
#include "timer.h"
#include "animation.h"

typedef struct Screen
{
    SDL_Renderer* renderer;
    SDL_Window* window;
} Screen;

typedef struct Shared_Library
{
    void* handle;
    char* name;
    intmax_t creation_time;
    int id;
    int size;
    char* fn_name;
    struct stat stats;
} Shared_Library;

typedef struct Memory
{
    bool is_initialized;
    bool wants_texture_refresh;

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
    Timer* timer;
    Animation* animation1;
} State;


#endif
