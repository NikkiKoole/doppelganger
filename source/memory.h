#ifndef MEMORY_H
#define MEMORY_H

#include <sys/stat.h>   //struct stat
#include <sys/types.h>  //struct stat
#include <stdint.h>     //int8_t et al.

#include "texture.h"
#include "defines.h"

typedef struct Screen
{
    SDL_Renderer* renderer;
    SDL_Window* window;
} Screen;

typedef struct SharedLibrary
{
    void* handle;
    char* name;
    intmax_t creation_time;
    int id;
    int size;
    char* fn_name;
    struct stat stats;
} SharedLibrary;

typedef struct Memory
{
    bool is_initialized;
    bool wants_texture_refresh;

	uint32 permanent_storage_size;
    void *permanent_storage;

    uint32 transient_storage_size;
	void *transient_storage;
} Memory;

typedef struct MemoryArena
{
    memory_index size;
    uint8 *base;
    memory_index used;
} MemoryArena;


// TODO : expand this babe to greatness!
typedef struct GameState
{
    MemoryArena world_arena;
    double angle1;
    Texture* tex1;
} GameState;


#endif
