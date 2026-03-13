typedef struct
{
    float x;
    float y;
} Direction;

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <math.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_FRect rect;

static Direction direction;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define SQUARE_WIDTH 20

#define SQUARE_TIME_TO_MOVE 500

#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

#define TICK_DELTA now - previousTick

#define SPEED 10

static float speed;
static Uint64 previousTick = 0;

int collisionCheck(float x, float y);
void randomizeDirection();
void setCoords(SDL_FRect *rect, Uint64 tickDelta);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Rectangles", "1.0", "com.example.renderer-rectangles");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Bouncing Square", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // setDirection();
    direction.x = direction.y = 0.0f;
    speed = (float)SQUARE_WIDTH / (float)SQUARE_TIME_TO_MOVE;

    rect.h = rect.w = 20.0f;
    rect.x = (WINDOW_WIDTH - 20.0f) / 2;
    rect.y = (WINDOW_HEIGHT - 20.0f) / 2;

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_LEFT)
        {
            direction.x = -1.0f;
            direction.y = 0.0f;
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        if (event->key.key == SDLK_LEFT)
        {
            direction.x = direction.y = 0.0f;
        }
    }

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const Uint64 now = SDL_GetTicks();

    if (collisionCheck(rect.x, rect.y))
        randomizeDirection();

    setCoords(&rect, TICK_DELTA);

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer); /* put it all on the screen! */

    previousTick = now;

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

int collisionCheck(float x, float y)
{
    return (x <= 0 || x >= (WINDOW_WIDTH - SQUARE_WIDTH) || y <= 0 || y >= (WINDOW_HEIGHT - SQUARE_WIDTH)) ? 1 : 0;
}

void randomizeDirection()
{
    srand((int)SDL_GetTicks());
    int dir = rand() % 8;

    switch (dir)
    {
    case NORTH:
        direction.x = 0.0f;
        direction.y = -1.0f;
        break;
    case NORTHEAST:
        direction.x = sqrtf(0.5f);
        direction.y = -sqrtf(0.5f);
        break;
    case EAST:
        direction.x = 1.0f;
        direction.y = 0.0f;
        break;
    case SOUTHEAST:
        direction.x = sqrtf(0.5f);
        direction.y = sqrtf(0.5f);
        break;
    case SOUTH:
        direction.x = 0.0f;
        direction.y = 1.0f;
        break;
    case SOUTHWEST:
        direction.x = -sqrtf(0.5f);
        direction.y = sqrtf(0.5f);
        break;
    case WEST:
        direction.x = -1.0f;
        direction.y = 0.0f;
        break;
    case NORTHWEST:
        direction.x = -sqrtf(0.5f);
        direction.y = -sqrtf(0.5f);
        break;
    }
}

void setCoords(SDL_FRect *rect, Uint64 tickDelta)
{
    rect->x += (tickDelta * speed * direction.x);
    rect->y += (tickDelta * speed * direction.y);

    if (rect->x <= 0)
        rect->x = 0.0f;
    else if (rect->x >= (WINDOW_WIDTH - SQUARE_WIDTH))
        rect->x = WINDOW_WIDTH - SQUARE_WIDTH;

    if (rect->y <= 0)
        rect->y = 0.0f;
    else if (rect->y >= (WINDOW_HEIGHT - SQUARE_WIDTH))
        rect->y = WINDOW_HEIGHT - SQUARE_WIDTH;
}