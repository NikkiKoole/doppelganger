#include "memory.h"


void* push_size_(Memory_Arena *arena, memory_index size){
    // TODO: this allignment should be an argument to this function with a default of 4.
    memory_index allignment = 4;

    memory_index result_pointer = (memory_index)arena->base + arena->used;
    memory_index allignment_offset = 0;

    memory_index allignment_mask = allignment - 1;
    if (result_pointer & allignment_mask){
        allignment_offset = allignment - (result_pointer & allignment_mask);

    }
    size += allignment_offset;

    ASSERT(arena->used + size <= arena->size);
    arena->used += size;
    void *result = (void *)(result_pointer + allignment_offset);
    return result;
}

void initialize_arena(Memory_Arena *arena, memory_index size, uint8 *base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}
