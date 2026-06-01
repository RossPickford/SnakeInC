#include "arena.h"

void createArena(Arena *arena, size_t size)
{
    if (!(arena->start = SDL_malloc(size)))
        SDL_Log("failed to allocate memory for arena");
    arena->size = size;
    arena->offset = 0;
}

void *arenaAlloc(Arena *arena, size_t size)
{
    // SDL_Log("%d", arena->offset);

    if ((arena->offset + size) >= arena->size)
    {
        SDL_Log("Cannot allocate due to capacity reached");
        return NULL;
    }

    void *current = (char *)arena->start + arena->offset;
    arena->offset += size;
    return current;
}

void freeArena(Arena *arena)
{
    arena->offset = 0;
}
void destroyArena(Arena *arena)
{
    SDL_free(arena->start);
}