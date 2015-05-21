#include <stdio.h>
#include <stdint.h>   //int8_t et al.
#include <sys/mman.h> //mmap
#include <stdbool.h>  //bool
#include <assert.h>


#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define Assert(Expression) if(!(Expression)) {printf("\n\nASSERT FAILED in function %s on line:%d (%s)\n\n",__FUNCTION__, __LINE__, __FILE__);*(int *)0 = 0;}

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

typedef struct world
{
    int32 x;
    int32 y;
    int32 z;
    char* name;
} world;

typedef struct guy
{
    char* name;
} guy;

typedef struct tile_position
{
    int32 x;
    int32 y;
    int32 z;
} tile_position;

typedef struct game_state
{
    memory_arena WorldArena;
    world *World ;
    guy *Guys;
    world *World2 ;
    
    tile_position playerP;
} game_state;

game_memory *Memory;

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, count, type) (type *)PushSize_(Arena, (count)*sizeof(type))
void* PushSize_(memory_arena *Arena, memory_index Size){
    Assert(Arena->Used + Size <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    return Result;
}

void InitializeArena(memory_arena *Arena, memory_index Size, uint8 *Base)
{
    Arena->Size = Size;
    Arena->Base = Base;
    Arena->Used = 0;
}

void game_update_and_render( )
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;

    if (!Memory->isInitialized){
        GameState->playerP.x = 10;
        GameState->playerP.y = 10;
        GameState->playerP.z = 10;
        InitializeArena(&GameState->WorldArena, 
                        Memory->PermanentStorageSize - sizeof(game_state),
                        (uint8 *)Memory->PermanentStorage + sizeof(game_state));
        GameState->World = (world*) PushStruct(&GameState->WorldArena, world);
        printf("World Adress (should be retained): %p\n", &(GameState->World));
        world* w = GameState->World;
        printf("Adress of world pointer (could change): %p\n", &w);
        w->x = 23;
        w->y = 32;
        w->z = 1230;
        GameState->Guys = PushArray(&GameState->WorldArena, 7, guy);
        GameState->Guys[0].name = "Anton";
        GameState->Guys[1].name = "Bertje";
        GameState->Guys[2].name = "Charlie";
        GameState->Guys[3].name = "Derrek";
        GameState->Guys[4].name = "Eddie";
        GameState->Guys[5].name = "Frankie";
        GameState->Guys[6].name = "Gera";
        GameState->Guys[7].name = "Herman";
        GameState->Guys[8].name = "Ido";
        GameState->Guys[9].name = "Johan";
        GameState->World2 = (world*) PushStruct(&GameState->WorldArena, world);
        world* w2 = GameState->World2;
        w2->x = 123;
        w2->y = 456;
        w2->z = 789;
        printf("sizeof guys: %d", sizeof((GameState->Guys[10])));
        Memory->isInitialized = true;
    }
    printf("World %d, %d, %d\n",GameState->World->x, GameState->World->y, GameState->World->z);
    printf("World2 %d, %d, %d\n",GameState->World2->x, GameState->World2->y, GameState->World2->z);
    printf("Gera? %s\n",GameState->Guys[6].name);
    printf("Now we are flying!\n");
}


int main (){

    void *BaseAddress = (void *) Gigabytes(1);
    game_memory GameMemory = {};
    GameMemory.PermanentStorageSize = Megabytes(64);
    GameMemory.TransientStorageSize = Gigabytes(1);

    uint64 TotalStorageSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;
    GameMemory.PermanentStorage = mmap(BaseAddress, TotalStorageSize,
                                       PROT_READ | PROT_WRITE,
                                       MAP_ANON | MAP_PRIVATE,
                                       -1, 0);
    GameMemory.TransientStorage = (uint8*)(GameMemory.PermanentStorage) + GameMemory.PermanentStorageSize;
    GameMemory.isInitialized = false;
    Memory = &GameMemory;
    game_update_and_render(GameMemory);
    game_update_and_render(GameMemory);
}

