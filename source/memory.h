#ifndef MEMORY_H__
#define MEMORY_H__

#include <sys/stat.h>   //struct stat
#include <sys/types.h>  //struct stat
#include <stdint.h>     //int8_t et al.

#include "texture.h"
#include "defines.h"


typedef struct shared_library
{
    void* handle;
    char* name;
    intmax_t creation_time;
    int size;
    char* fn_name;
    struct stat stats;
} shared_library;

typedef struct game_memory
{
    bool isInitialized;
    bool wantsTextureRefresh;

	uint32 PermanentStorageSize;
    void *PermanentStorage;

    uint32 TransientStorageSize;
	void *TransientStorage;
} game_memory;

typedef struct memory_arena
{
    memory_index Size;
    uint8 *Base;
    memory_index Used;
} memory_arena;


// TODO : expand this babe to greatness!
typedef struct game_state
{
    memory_arena WorldArena;
    texture* tex1;
} game_state;


#endif
