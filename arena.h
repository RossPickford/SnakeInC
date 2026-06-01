#include <SDL3/SDL.h>

typedef struct Arena
{
    void *start;
    size_t size;
    size_t offset;
} Arena;

void createArena(Arena *arena, size_t size);
void *arenaAlloc(Arena *arena, size_t size);
void freeArena(Arena *arena);
void destroyArena(Arena *arena);