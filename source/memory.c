#include "memory.h"

void* push_size_(Memory_Arena *arena, memory_index size){
    ASSERT(arena->used + size <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    return result;
}

void initialize_arena(Memory_Arena *arena, memory_index size, uint8 *base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}
