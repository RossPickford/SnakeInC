// #include "arena.h"

/* void createArena(Arena *arena, size_t size)
{
    // arena->start = SDL_malloc(size);
    arena->size = size;
    arena->offset = 0;
}

void *ArenaAlloc(Arena *arena, size_t size)
{
    if ((arena->offset + size) > arena->size)
    {
        // SDL_Log("Cannot allocate due to capacity reached");
        return NULL;
    }

    arena->offset += size;
    void *current = arena->currentPos;
    arena->currentPos = (char *)arena->currentPos + (size - 1);
    return current;
} */