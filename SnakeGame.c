#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_FRect wallBackground;
static SDL_FRect mainBackground;
static SDL_FRect* snake;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define GRID_WIDTH 20

#define GREY 30, 30, 30

void setCoords(SDL_FRect *rect, Uint64 tickDelta);
void renderWalls();
void drawSnake();
void renderFood();
void collisionCheck();

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Snake Game in C", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    wallBackground.h = WINDOW_HEIGHT;
    wallBackground.w = WINDOW_WIDTH;
    wallBackground.x = wallBackground.y = 0.0f;

    mainBackground.h = WINDOW_HEIGHT - (2 * GRID_WIDTH);
    mainBackground.w = WINDOW_WIDTH - (2 * GRID_WIDTH);
    mainBackground.x = mainBackground.y = GRID_WIDTH;
    
    snake = (SDL_FRect*)malloc(sizeof(SDL_FRect) * (powf((WINDOW_HEIGHT - (GRID_WIDTH * 2) / (float) GRID_WIDTH), 2)));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    renderWalls();
    // drawSnake();

    SDL_RenderPresent(renderer);

    // previousTick = now;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    free(snake);
}

void setCoords(SDL_FRect *rect, Uint64 tickDelta)
{
}

void renderWalls()
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &wallBackground);

    SDL_SetRenderDrawColor(renderer, GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &mainBackground);
}