typedef struct
{
    int x;
    int y;
} Int_Vector2;

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_FRect wallBackground;
static SDL_FRect mainBackground;

static SDL_FRect *arena;
static SDL_FRect *snake;
static SDL_FRect *fruit;

static Uint64 arenaSize;
static Uint64 snakeLength = 3;
static Uint64 fruitSize = 3;

static Int_Vector2 dir;

static Uint64 previousTick = 0, tickDelta = 0;

#define CLOCK_SPEED 250

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define GRID_WIDTH 20

#define GREY 30, 30, 30

void initBackground();
void initSnakeAndFruit();

void moveSnake(SDL_FRect *head, Uint64 length);
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

    arenaSize = powf((WINDOW_HEIGHT - (GRID_WIDTH * 2) / (float)GRID_WIDTH), 2);
    snake = (SDL_FRect *)malloc(sizeof(SDL_FRect) * arenaSize);

    float coordx = (WINDOW_WIDTH / 2) - (WINDOW_WIDTH % GRID_WIDTH);
    float coordy = (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT % GRID_WIDTH);

    for (int i = 0; i < 3; i++)
    {
        (snake + i)->h = (snake + i)->w = GRID_WIDTH;
        (snake + i)->y = coordy;
        (snake + i)->x = coordx + (GRID_WIDTH * i);
    }

    dir.x = -1.0f;
    dir.y = 0.0f;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_LEFT && dir.x != 1)
        {
            dir.x = -1;
            dir.y = 0;
        }
        else if (event->key.key == SDLK_RIGHT && dir.x != -1)
        {
            dir.x = 1;
            dir.y = 0;
        }
        else if (event->key.key == SDLK_UP && dir.y != 1)
        {
            dir.x = 0;
            dir.y = -1;
        }
        else if (event->key.key == SDLK_DOWN && dir.y != -1)
        {
            dir.x = 0;
            dir.y = 1;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 currentTick = SDL_GetTicks();

    tickDelta += (currentTick - previousTick);

    if (tickDelta >= CLOCK_SPEED)
    {
        moveSnake(snake, 3);
        tickDelta -= CLOCK_SPEED;
    }

    previousTick = currentTick;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    renderWalls();
    drawSnake();

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    free(snake);
}

void initBackground()
{
}

void initSnakeAndFruit()
{
}

void moveSnake(SDL_FRect *head, Uint64 length)
{
    for (int i = length - 1; i >= 1; i--)
    {
        (head + i)->x = (head + i - 1)->x;
        (head + i)->y = (head + i - 1)->y;
    }

    head->x += (GRID_WIDTH * dir.x);
    head->y += (GRID_WIDTH * dir.y);
}

void renderWalls()
{
    SDL_SetRenderDrawColor(renderer, 64, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &wallBackground);

    SDL_SetRenderDrawColor(renderer, GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &mainBackground);
}

void drawSnake()
{
    SDL_SetRenderDrawColor(renderer, 15, 210, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, snake);
    SDL_SetRenderDrawColor(renderer, 15, 110, 0, SDL_ALPHA_OPAQUE);

    for (int i = 1; i < 3; i++)
    {
        SDL_RenderFillRect(renderer, (snake + i));
    }
}