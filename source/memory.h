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
#include "screen.h"
#include "blockmap.h"
#include "geom.h"

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

// TODO : expand this babe to greatness!

typedef struct State
{
    Memory_Arena world_arena;
    real64  angle1;
    Texture *tex1;
    Texture *terminal8;
    Texture *render_target;
    Texture *zelda;
    Texture *blocks;
    Timer *timer;
    Animation *animation1;
    Sprite *walking_left;
    Sprite *walking_right;
    World *world;
    Texture *world_slices;
} State;

// this is the function thats being called from main, offered up by the gamelib.so file.
void game_update_and_render(Screen *screen, Memory *memory, Keyboard *keyboard, FrameTime *frametime);

#endif
