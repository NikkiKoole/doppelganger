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
    SDL_Renderer *renderer;
    SDL_Window *window;
} Screen;

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


// TODO : expand this babe to greatness!

typedef struct Block
{
    uint8 type;
} Block;

#define WORLD_WIDTH    32
#define WORLD_HEIGHT   32
#define WORLD_DEPTH    32

typedef struct World
{
    Block blocks[WORLD_WIDTH  *WORLD_HEIGHT  *WORLD_DEPTH];
} World;

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
