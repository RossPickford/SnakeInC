typedef struct
{
    int x;
    int y;
} Int_Vector2;

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static char *title = "SNAKE";
static SDL_Texture *titleTexture = NULL;
static TTF_Font *titleFont = NULL;
static SDL_FRect startBtn;

static char *startTxt = "START";

static SDL_FRect wallBackground;
static SDL_FRect mainBackground;

static SDL_FRect *arena;
static SDL_FRect *snake;
static SDL_FRect *fruit;
static SDL_FRect *bufferRect;

static Uint64 arenaSize;
static Uint64 snakeLength = 3;
static Uint64 fruitSize = 3;

static Int_Vector2 dir;
static Int_Vector2 prevHeadLocation;

static Uint64 previousTick = 0, tickDelta = 0;
static Uint64 previousEventTick = 0;

#define GAME_SPEED_MULTIPLIER 1
#define TICK_RATE_MILLISECONDS 250
#define INPUT_THRESHOLD 150

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define GRID_WIDTH 20

#define MAX_HORIZONTAL_WALL WINDOW_WIDTH - GRID_WIDTH
#define MAX_VERTICAL_WALL WINDOW_HEIGHT - GRID_WIDTH

#define GREY 30, 30, 30
#define SNAKE_HEAD_COLOUR 15, 210, 0
#define SNAKE_BODY_COLOUR 15, 110, 0
#define RED 255, 0, 0

bool initMainMenu(SDL_AppResult *result);
bool initGameStart(SDL_AppResult *result);
bool initGameLoop(SDL_AppResult *result);
bool initGamePause(SDL_AppResult *result);
bool initGameOver(SDL_AppResult *result);

void initBackground();
void initSnakeAndFruit();

SDL_AppResult MainMenu_Input(void *appstate, SDL_Event *event);
SDL_AppResult GameStart_Input(void *appstate, SDL_Event *event);
SDL_AppResult GameLogic_Input(void *appstate, SDL_Event *event);
SDL_AppResult GamePause_Input(void *appstate, SDL_Event *event);
SDL_AppResult GameOver_Input(void *appstate, SDL_Event *event);

SDL_AppResult MainMenu_Loop(void *appstate);
SDL_AppResult GameStart_Loop(void *appstate);
SDL_AppResult GameLogic_Loop(void *appstate);
SDL_AppResult GamePause_Loop(void *appstate);
SDL_AppResult GameOver_Loop(void *appstate);

void moveSnake(SDL_FRect *head, Uint64 length);
void renderWalls();
void drawSnake();
void drawFruit();

int collisionCheck(SDL_FRect *head);
int snakeEatFruitCheck(SDL_FRect head, SDL_FRect **eatenFruit);
int lengthenSnake(SDL_FRect *head, Uint64 *length, SDL_FRect *eatenFruit);
Int_Vector2 getRandomCoord();

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

    if (!TTF_Init())
    {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_AppResult result = SDL_APP_CONTINUE;

    if (!initMainMenu(&result))
        return result;

    initBackground();

    arenaSize = powf(((WINDOW_HEIGHT - (GRID_WIDTH * 2)) / (float)GRID_WIDTH), 2);
    arena = snake = (SDL_FRect *)malloc(sizeof(SDL_FRect) * (arenaSize + 1 /*buffer rect*/));
    bufferRect = arena + arenaSize;
    fruit = snake + snakeLength;

    initSnakeAndFruit();

    dir.x = -1;
    dir.y = 0;

    prevHeadLocation.x = prevHeadLocation.y = 0;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    return GameLogic_Input(appstate, event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // SDL_AppResult result = GameLogic_Loop(appstate);
    SDL_AppResult result = MainMenu_Loop(appstate);
    SDL_RenderPresent(renderer);

    return result;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    free(snake);
}

//===================================================================================

bool initMainMenu(SDL_AppResult *result)
{
    SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE};
    SDL_Surface *titleText;

    titleFont = TTF_OpenFont("./fonts/Sunglass_one.otf", 150.0f);
    if (!titleFont)
    {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        *result = SDL_APP_FAILURE;
        return false;
    }

    /* Create the text */
    titleText = TTF_RenderText_Blended(titleFont, "SNAKE", 0, color);
    if (titleText)
    {
        titleTexture = SDL_CreateTextureFromSurface(renderer, titleText);
        SDL_DestroySurface(titleText);
    }
    if (!titleTexture)
    {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        *result = SDL_APP_FAILURE;
        return false;
    }

    return true;
}

SDL_AppResult MainMenu_Input(void *appstate, SDL_Event *event)
{
}

SDL_AppResult GameLogic_Input(void *appstate, SDL_Event *event)
{
    // Uint64 eventTick = SDL_GetTicks();

    // if ((eventTick - previousEventTick) < INPUT_THRESHOLD)
    // return SDL_APP_CONTINUE;

    // previousEventTick = eventTick;

    if ((int)snake->x == prevHeadLocation.x && (int)snake->y == prevHeadLocation.y)
        return SDL_APP_CONTINUE;

    prevHeadLocation.x = (int)snake->x;
    prevHeadLocation.y = (int)snake->y;

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Log("key pressed %d", event->key.scancode);
        if (event->key.scancode == SDL_SCANCODE_LEFT && dir.x != 1)
        {
            dir.x = -1;
            dir.y = 0;
        }
        else if (event->key.scancode == SDL_SCANCODE_RIGHT && dir.x != -1)
        {
            dir.x = 1;
            dir.y = 0;
        }
        else if (event->key.scancode == SDL_SCANCODE_UP && dir.y != 1)
        {
            dir.x = 0;
            dir.y = -1;
        }
        else if (event->key.scancode == SDL_SCANCODE_DOWN && dir.y != -1)
        {
            dir.x = 0;
            dir.y = 1;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult MainMenu_Loop(void *appdstate)
{
    int w = 0, h = 0;
    SDL_FRect dst;
    const float scale = 1.0f;

    /* Center the text and scale it up */
    // SDL_GetRenderOutputSize(renderer, &w, &h);
    // SDL_SetRenderScale(renderer, scale, scale);
    SDL_GetTextureSize(titleTexture, &dst.w, &dst.h);
    dst.x = (WINDOW_WIDTH - dst.w) / 2;
    dst.y = (WINDOW_HEIGHT - dst.h) / 10;

    const double now = ((double)SDL_GetTicks()) / 1000.0; /* convert from milliseconds to seconds. */
    /* choose the modulation values for the center texture. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin((now * 2) + SDL_PI_D * 4 / 3));

    /* Draw the text */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetTextureColorModFloat(titleTexture, red, green, blue);
    SDL_RenderTexture(renderer, titleTexture, NULL, &dst);

    return SDL_APP_CONTINUE;
}

SDL_AppResult GameLogic_Loop(void *appstate)
{
    Uint64 currentTick = SDL_GetTicks();

    tickDelta += (currentTick - previousTick);

    if (tickDelta >= (TICK_RATE_MILLISECONDS / GAME_SPEED_MULTIPLIER))
    {
        tickDelta -= (TICK_RATE_MILLISECONDS / GAME_SPEED_MULTIPLIER);

        int fruitEatenCheck = 0;

        SDL_FRect pseudoHead = *snake;

        pseudoHead.x += ((GRID_WIDTH / GAME_SPEED_MULTIPLIER) * dir.x);
        pseudoHead.y += ((GRID_WIDTH / GAME_SPEED_MULTIPLIER) * dir.y);

        SDL_FRect **eatenFruit = &bufferRect;
        if (snakeEatFruitCheck(pseudoHead, eatenFruit))
        {
            if (!eatenFruit)
            {
                SDL_Log("pointer is null");
                return SDL_APP_FAILURE;
            }

            if (!lengthenSnake(snake, &snakeLength, *eatenFruit))
                return SDL_APP_FAILURE;

            fruitEatenCheck = 1;
        }

        moveSnake(snake, snakeLength);

        if (!fruitEatenCheck && collisionCheck(snake))
            return SDL_APP_SUCCESS;
    }

    previousTick = currentTick;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    renderWalls();
    drawFruit();
    drawSnake();

    return SDL_APP_CONTINUE;
}

void initBackground()
{
    wallBackground.h = WINDOW_HEIGHT;
    wallBackground.w = WINDOW_WIDTH;
    wallBackground.x = wallBackground.y = 0.0f;

    mainBackground.h = WINDOW_HEIGHT - (2 * GRID_WIDTH);
    mainBackground.w = WINDOW_WIDTH - (2 * GRID_WIDTH);
    mainBackground.x = mainBackground.y = GRID_WIDTH;
}

void initSnakeAndFruit()
{
    float coordx = (WINDOW_WIDTH / 2) - (WINDOW_WIDTH % GRID_WIDTH);
    float coordy = (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT % GRID_WIDTH);

    for (int i = 0; i < snakeLength; i++)
    {
        (snake + i)->h = (snake + i)->w = GRID_WIDTH;
        (snake + i)->y = coordy;
        (snake + i)->x = coordx + (GRID_WIDTH * i);
    }

    for (int i = 0; i < fruitSize; i++)
    {
        (fruit + i)->h = (fruit + i)->w = GRID_WIDTH;

        Int_Vector2 randCoord = getRandomCoord();
        // will need a check to make sure the coordinate does not overlap with a previously generated one.
        (fruit + i)->y = (float)randCoord.y;
        (fruit + i)->x = (float)randCoord.x;
    }
}

int collisionCheck(SDL_FRect *head) // don't need to do collision check if eating fruit was succesful
{
    if (head->x == 0 || head->x == MAX_HORIZONTAL_WALL || head->y == 0 || head->y == MAX_VERTICAL_WALL)
        return 1;

    for (int i = 4; i < snakeLength; i++)
        if ((int)snake->x == (int)(snake + i)->x && (int)snake->y == (int)(snake + i)->y)
            return 1;

    return 0;
}

int snakeEatFruitCheck(SDL_FRect head, SDL_FRect **eatenFruit)
{
    for (int i = 0; i < fruitSize; i++)
        if ((int)head.x == (int)(fruit + i)->x && (int)head.y == (int)(fruit + i)->y)
        {
            SDL_Log("fruit eaten");
            *eatenFruit = (fruit + i);
            return 1;
        }

    return 0;
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
    SDL_SetRenderDrawColor(renderer, SNAKE_HEAD_COLOUR, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, snake);
    SDL_SetRenderDrawColor(renderer, SNAKE_BODY_COLOUR, SDL_ALPHA_OPAQUE);

    for (int i = 1; i < snakeLength; i++)
        SDL_RenderFillRect(renderer, (snake + i));
}

void drawFruit()
{
    SDL_SetRenderDrawColor(renderer, RED, SDL_ALPHA_OPAQUE);

    for (int i = 0; i < fruitSize; i++)
        SDL_RenderFillRect(renderer, (fruit + i));
}

Int_Vector2 getRandomCoord()
{
    Int_Vector2 coord;
    int x = SDL_rand(MAX_HORIZONTAL_WALL - GRID_WIDTH);
    int y = SDL_rand(MAX_VERTICAL_WALL - GRID_WIDTH);

    x -= x % GRID_WIDTH;
    y -= y % GRID_WIDTH;

    coord.x = x + GRID_WIDTH;
    coord.y = y + GRID_WIDTH;

    return coord;
}

int lengthenSnake(SDL_FRect *head, Uint64 *length, SDL_FRect *eatenFruit)
{
    if (fruitSize + ++*length >= arenaSize)
        return 0;

    // if (!eatenFruit)
    // SDL_Log("pointer is null");

    Int_Vector2 randCoord = getRandomCoord();
    // will need a check to make sure the coordinate does not overlap with a previously generated one.
    eatenFruit->y = (float)randCoord.y;
    eatenFruit->x = (float)randCoord.x;

    for (int i = fruitSize; i > 0; i--)
    {
        *(fruit + i) = *(fruit + i - 1);
        *(fruit + i) = *(fruit + i - 1);
    }

    fruit++;
    return 1;
}