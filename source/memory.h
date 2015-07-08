#ifndef MEMORY_H
#define MEMORY_H

#include <sys/stat.h>   //struct stat
#include <sys/types.h>  //struct stat
#include <stdbool.h>    // bool
#include <stdio.h>
#include "defines.h"

typedef struct Shared_Library
{
    void *handle;
    const char *name;
    intmax_t creation_time;
    uint32 id;
    uint32 size;
    const char *fn_name;
    struct stat stats;
} Shared_Library;

typedef struct FrameTime
{
    uint32 duration;
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

typedef struct TempMemory
{
    Memory_Arena *arena;
    memory_index used;
} TempMemory;

#define PUSH_STRUCT(arena, type) (type *)push_size_(arena, sizeof(type))
#define PUSH_ARRAY(arena, count, type) (type *)push_size_(arena, (count)*sizeof(type))
#define PUSH_SIZE(arena, size) push_size_(arena, size)

void* push_size_(Memory_Arena *arena, memory_index size);
void initialize_arena(Memory_Arena *arena, memory_index size, uint8 *base);
void end_temporary_memory(TempMemory temp, Memory_Arena* arena);
TempMemory begin_temporary_memory(Memory_Arena* arena);

typedef struct TransState
{
    Memory_Arena scratch_arena;
} TransState;

#endif
