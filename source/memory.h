#ifndef MEMORY_H__
#define MEMORY_H__

#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>   //int8_t et al.

#include "texture.h"

#define internal static

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define Assert(Expression) if(!(Expression)) {printf("\nASSERT FAILED in function %s on line:%d (%s)\n\n",__FUNCTION__, __LINE__, __FILE__);exit(0);}

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef size_t memory_index;


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
