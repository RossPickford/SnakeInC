/* rectangles.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some
 * rectangles to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

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

#define SECOND_IN_MILLISECONDS 1000

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

static float dist;
static Uint64 previousTick = 0;

int collisionCheck(float x, float y);
void setDirection();
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

    setDirection();
    dist = SECOND_IN_MILLISECONDS / SQUARE_WIDTH;

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
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const Uint64 now = SDL_GetTicks();

    if (collisionCheck(rect.x, rect.y))
        setDirection();

    setCoords(&rect, TICK_DELTA);

        /* we'll have the rectangles grow and shrink over a few seconds. */
        // const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
        // const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

        /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); /* black, full alpha */
    SDL_RenderClear(renderer);                                       /* start with a blank canvas. */

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    /* Rectangles are comprised of set of X and Y coordinates, plus width and
       height. (0, 0) is the top left of the window, and larger numbers go
       down and to the right. This isn't how geometry works, but this is
       pretty standard in 2D graphics. */

    // /* Let's draw a single rectangle (square, really). */
    // rect[0].x = rect[0].y = 100;
    // rect[0].w = rect[0].h = 100 + (100 * scale);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    // SDL_RenderRect(renderer, &rect[0]);

    // /* Now let's draw several rectangles with one function call. */
    // for (i = 0; i < 3; i++) {
    //     const float size = (i+1) * 50.0f;
    //     rect[i].w = rect[i].h = size + (size * scale);
    //     rect[i].x = (WINDOW_WIDTH - rect[i].w) / 2;  /* center it. */
    //     rect[i].y = (WINDOW_HEIGHT - rect[i].h) / 2;  /* center it. */
    // }
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);  /* green, full alpha */
    // SDL_RenderRects(renderer, rect, 3);  /* draw three rectangles at once */

    // /* those were rectangle _outlines_, really. You can also draw _filled_ rectangles! */
    // rect[0].x = 400;
    // rect[0].y = 50;
    // rect[0].w = 100 + (100 * scale);
    // rect[0].h = 50 + (50 * scale);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    // SDL_RenderFillRect(renderer, &rect[0]);

    // /* ...and also fill a bunch of rectangles at once... */
    // for (i = 0; i < SDL_arraysize(rect); i++) {
    //     const float w = (float) (WINDOW_WIDTH / SDL_arraysize(rect));
    //     const float h = i * 8.0f;
    //     rect[i].x = i * w;
    //     rect[i].y = WINDOW_HEIGHT - h;
    //     rect[i].w = w;
    //     rect[i].h = h;
    // }
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    // SDL_RenderFillRects(renderer, rect, SDL_arraysize(rect));

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

void setDirection()
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
    rect->x += ((tickDelta / dist) * direction.x * SPEED);
    rect->y += ((tickDelta / dist) * direction.y * SPEED);

    if (rect->x <= 0) rect->x = 0.0f;
    else if (rect->x >= (WINDOW_WIDTH - SQUARE_WIDTH)) rect->x = WINDOW_WIDTH - SQUARE_WIDTH;

    if (rect->y <= 0) rect->y = 0.0f;
    else if (rect->y >= (WINDOW_HEIGHT - SQUARE_WIDTH)) rect->y = WINDOW_HEIGHT - SQUARE_WIDTH;
}