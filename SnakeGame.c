#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <math.h>

typedef enum
{
    MAIN_MENU,
    GAME_START,
    GAME_LOOP,
    GAME_PAUSE,
    GAME_OVER,
} E_GameState;

typedef enum
{
    PRESSED,
    RELEASED,
    HOVERING,
    NORMAL,
    NONE,
} E_ButtonState;

typedef struct
{
    int x;
    int y;
} Int_Vector2;

typedef struct
{
    char *text;
    SDL_Texture *texture;
    TTF_Font *font;
    float fontSize;
    SDL_Color colour;
    SDL_FRect rect;
} TextData;

typedef struct
{
    TextData textData;
    SDL_Color displayColour;
    SDL_Color highlightColour;
    SDL_Color selectColour;
    E_ButtonState currentState;
    E_ButtonState previousState;
    bool pressed;
} ButtonData;

typedef struct
{
    float x;
    float y;
    float height;
    float width;
    float position;
    float normalisedPos;
} SliderData;

static E_GameState currentGameState = MAIN_MENU;

static SDL_FPoint mousePos;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static TTF_Font *titleFont = NULL;
static TTF_Font *mainFont = NULL;

static TextData MM_titleData;
static char *MM_title = "SNAKE";

static ButtonData MM_startBtnData;
static char *MM_startTxt = "START";

static TextData MM_difficultyData;
static ButtonData MM_difficultyBtnData;
static char *MM_difficultyTxt = "DIFFICULTY ";
static char *MM_difficultyEasyTxt = "EASY  ";
static char *MM_difficultyMediumTxt = "MEDIUM";
static char *MM_difficultyHardTxt = "HARD  ";
static char *nextDifficulty;
static bool changingDifficulty = false;

static TextData MM_volumeData;
static ButtonData MM_volumeBtnData;
static SliderData MM_volumeSliderData;
static char *MM_volumeText = "VOLUME     ";

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

static Uint64 tickRateMilliseconds = 0;
static Uint64 previousTick = 0, tickDelta = 0;
static Uint64 previousEventTick = 0;

#define GAME_SPEED_MULTIPLIER 1
#define TICK_RATE_MILLISECONDS 250
#define INPUT_THRESHOLD 150

#define EASY 250
#define MEDIUM 175
#define HARD 100

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
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

void drawVolumeBar(ButtonData *btnData, SliderData *sliderData);

void moveSnake(SDL_FRect *head, Uint64 length);
void renderWalls();
void drawSnake();
void drawFruit();

int collisionCheck(SDL_FRect *head);
int snakeEatFruitCheck(SDL_FRect head, SDL_FRect **eatenFruit);
int lengthenSnake(SDL_FRect *head, Uint64 *length, SDL_FRect *eatenFruit);
Int_Vector2 getRandomCoord();

bool InitText(TextData *textData);
bool moveDifficultText(TextData *txtData, TextData *temp, float speed, float timeDelta, char *nextText);
SDL_Color getButtonTextColour(ButtonData *btnData);
bool ChangeButtonState(ButtonData *btnData, E_ButtonState btnState);
bool CheckButtonState(ButtonData *btnData, SDL_Event *event);

void changeVolume(SliderData *slderData, float mouseX);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Snake Game in C", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer))
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

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_ESCAPE)
            return SDL_APP_SUCCESS;
    }

    switch (currentGameState)
    {
    case MAIN_MENU:
        return MainMenu_Input(appstate, event);
    case GAME_LOOP:
        return GameLogic_Input(appstate, event);
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_AppResult result;

    switch (currentGameState)
    {
    case MAIN_MENU:
        result = MainMenu_Loop(appstate);
        break;
    case GAME_LOOP:
        result = GameLogic_Loop(appstate);
        break;
    }

    SDL_RenderPresent(renderer);

    return result;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    free(arena);
}

//=================================================================================== MAIN MENU

bool initMainMenu(SDL_AppResult *result)
{
    SDL_Color colour_black = {0, 0, 0, SDL_ALPHA_OPAQUE};
    SDL_Color colour_white = {255, 255, 255, SDL_ALPHA_OPAQUE};
    SDL_Color colour_red = {255, 0, 0, SDL_ALPHA_OPAQUE};
    SDL_Color colour_green = {0, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Color colour_blue = {0, 0, 255, SDL_ALPHA_OPAQUE};
    SDL_Color colour_yellow = {255, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Surface *titleText;
    SDL_Surface *startText;

    titleFont = TTF_OpenFont("./fonts/Sunglass_one.otf", 150.0f);
    mainFont = TTF_OpenFont("./fonts/PublicPixel.ttf", 20.0f);
    if (!titleFont || !mainFont)
    {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        *result = SDL_APP_FAILURE;
        return false;
    }

    MM_titleData.text = MM_title;
    MM_titleData.font = titleFont;
    MM_titleData.fontSize = 150.0f;
    MM_titleData.colour = colour_black;

    MM_startBtnData.textData.text = MM_startTxt;
    MM_startBtnData.textData.font = mainFont;
    MM_startBtnData.textData.fontSize = 20.0f;
    MM_startBtnData.currentState = NORMAL;
    MM_startBtnData.previousState = NONE;
    MM_startBtnData.displayColour = MM_startBtnData.textData.colour = colour_white;
    MM_startBtnData.highlightColour = colour_green;
    MM_startBtnData.selectColour = colour_red;

    MM_difficultyBtnData.textData.text = MM_difficultyTxt;
    MM_difficultyBtnData.textData.font = mainFont;
    MM_difficultyBtnData.textData.fontSize = 20.0f;
    MM_difficultyBtnData.currentState = NORMAL;
    MM_difficultyBtnData.previousState = NONE;
    MM_difficultyBtnData.displayColour = MM_difficultyBtnData.textData.colour = colour_white;
    MM_difficultyBtnData.highlightColour = colour_green;
    MM_difficultyBtnData.selectColour = colour_red;

    MM_difficultyData.text = MM_difficultyEasyTxt;
    MM_difficultyData.font = mainFont;
    MM_difficultyData.fontSize = 20.0f;
    MM_difficultyData.colour = colour_blue;

    MM_volumeData.text = MM_volumeText;
    MM_volumeData.font = mainFont;
    MM_volumeData.fontSize = 20.0f;
    MM_volumeData.colour = colour_white;

    MM_volumeBtnData.currentState = NORMAL;
    MM_volumeBtnData.previousState = NONE;
    MM_volumeBtnData.textData.colour = MM_volumeBtnData.displayColour = colour_green;
    MM_volumeBtnData.highlightColour = colour_yellow;
    MM_volumeBtnData.selectColour = colour_red;

    /* Create the text */
    if (!InitText(&MM_titleData) || !InitText(&MM_startBtnData.textData) || !InitText(&MM_difficultyData) || !InitText(&MM_difficultyBtnData.textData) || !InitText(&MM_volumeData))
    {
        *result = SDL_APP_FAILURE;
        return false;
    }

    tickRateMilliseconds = EASY;

    /* Set Title position */
    SDL_GetTextureSize(MM_titleData.texture, &MM_titleData.rect.w, &MM_titleData.rect.h);
    MM_titleData.rect.x = (WINDOW_WIDTH - MM_titleData.rect.w) / 2;
    MM_titleData.rect.y = (WINDOW_HEIGHT - MM_titleData.rect.h) / 10;

    /* Set Start button position */
    SDL_GetTextureSize(MM_startBtnData.textData.texture, &MM_startBtnData.textData.rect.w, &MM_startBtnData.textData.rect.h);
    MM_startBtnData.textData.rect.x = (WINDOW_WIDTH - MM_startBtnData.textData.rect.w) / 2;
    MM_startBtnData.textData.rect.y = (WINDOW_HEIGHT - MM_startBtnData.textData.rect.h) / 2;

    /* Set Difficulty text and button position */
    SDL_GetTextureSize(MM_difficultyBtnData.textData.texture, &MM_difficultyBtnData.textData.rect.w, &MM_difficultyBtnData.textData.rect.h);
    SDL_GetTextureSize(MM_difficultyData.texture, &MM_difficultyData.rect.w, &MM_difficultyData.rect.h);

    MM_difficultyBtnData.textData.rect.x = (WINDOW_WIDTH - (MM_difficultyBtnData.textData.rect.w + MM_difficultyData.rect.w)) / 2;
    MM_difficultyBtnData.textData.rect.y = MM_startBtnData.textData.rect.y + (MM_startBtnData.textData.rect.h * 1.5f);
    MM_difficultyData.rect.x = 0.0f; // Coords are at 0,0 as this will be displayed through a viewport.
    MM_difficultyData.rect.y = 0.0f;

    /* Set Volume text and slider position*/
    SDL_GetTextureSize(MM_volumeData.texture, &MM_volumeData.rect.w, &MM_volumeData.rect.h);
    MM_volumeBtnData.textData.rect.h = MM_volumeSliderData.height = MM_volumeData.rect.h;
    MM_volumeBtnData.textData.rect.w = MM_volumeSliderData.width = MM_difficultyData.rect.w;
    MM_volumeData.rect.x = (WINDOW_WIDTH - (MM_volumeData.rect.w + MM_volumeSliderData.width)) / 2;
    MM_volumeData.rect.y = MM_difficultyBtnData.textData.rect.y + (MM_difficultyBtnData.textData.rect.h * 1.5f);
    MM_volumeBtnData.textData.rect.x = MM_volumeSliderData.x = MM_volumeData.rect.x + MM_volumeData.rect.w;
    MM_volumeBtnData.textData.rect.y = MM_volumeSliderData.y = MM_volumeData.rect.y;
    MM_volumeSliderData.position = MM_volumeSliderData.x + (MM_volumeSliderData.width / 2.0f);

    return true;
}

SDL_AppResult MainMenu_Input(void *appstate, SDL_Event *event)
{
    SDL_ConvertEventToRenderCoordinates(renderer, event);

    mousePos.x = event->button.x;
    mousePos.y = event->button.y;

    if (CheckButtonState(&MM_startBtnData, event) && MM_startBtnData.currentState == RELEASED)
        currentGameState = GAME_LOOP;

    if (CheckButtonState(&MM_difficultyBtnData, event) && MM_difficultyBtnData.currentState == RELEASED)
    {
        if (!changingDifficulty)
        {
            nextDifficulty = MM_difficultyData.text == MM_difficultyEasyTxt ? MM_difficultyMediumTxt : MM_difficultyData.text == MM_difficultyMediumTxt ? MM_difficultyHardTxt
                                                                                                                                                        : MM_difficultyEasyTxt;
            // Set the difficulty speed
            if (nextDifficulty == MM_difficultyEasyTxt)
                tickRateMilliseconds = EASY;
            if (nextDifficulty == MM_difficultyMediumTxt)
                tickRateMilliseconds = MEDIUM;
            if (nextDifficulty == MM_difficultyHardTxt)
                tickRateMilliseconds = HARD;

            changingDifficulty = true;
        }
    }

    CheckButtonState(&MM_volumeBtnData, event);
    if (MM_volumeBtnData.currentState == PRESSED)
        changeVolume(&MM_volumeSliderData, mousePos.x);

    return SDL_APP_CONTINUE;
}

SDL_AppResult MainMenu_Loop(void *appdstate)
{
    const Uint64 currentTick = (Uint64)SDL_GetTicks();
    tickDelta = currentTick - previousTick;
    previousTick = currentTick;

    double DtickDelta = tickDelta / 1000.0; // convert to seconds

    SDL_Rect diffViewport;

    SDL_Color Cgreen = {0, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};

    /* update button text if the state has changed */
    if (MM_startBtnData.currentState != MM_startBtnData.previousState)
        InitText(&MM_startBtnData.textData);

    if (MM_difficultyBtnData.currentState != MM_difficultyBtnData.previousState)
        InitText(&MM_difficultyBtnData.textData);

    if (MM_volumeBtnData.currentState == RELEASED)
        CheckButtonState(&MM_volumeBtnData, NULL);

    const double now = currentTick / 1000.0; /* convert from milliseconds to seconds. */
    /* choose the modulation values for the center texture. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin((now * 2) + SDL_PI_D * 4 / 3));

    /*set the viewport size*/
    diffViewport.x = MM_difficultyBtnData.textData.rect.x + MM_difficultyBtnData.textData.rect.w;
    diffViewport.y = MM_difficultyBtnData.textData.rect.y;
    diffViewport.w = MM_difficultyData.rect.w;
    diffViewport.h = MM_difficultyData.rect.h;

    /* Draw the title */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetTextureColorModFloat(MM_titleData.texture, red, green, blue);
    SDL_RenderTexture(renderer, MM_titleData.texture, NULL, &MM_titleData.rect);

    SDL_RenderTexture(renderer, MM_startBtnData.textData.texture, NULL, &MM_startBtnData.textData.rect);
    SDL_RenderTexture(renderer, MM_difficultyBtnData.textData.texture, NULL, &MM_difficultyBtnData.textData.rect);

    /* Draw difficulty buttons */
    SDL_SetRenderViewport(renderer, &diffViewport);
    if (changingDifficulty)
    {
        CheckButtonState(&MM_difficultyBtnData, NULL);
        TextData temp;
        changingDifficulty = moveDifficultText(&MM_difficultyData, &temp, 350, DtickDelta, nextDifficulty);
        SDL_RenderTexture(renderer, temp.texture, NULL, &temp.rect);
    }
    SDL_RenderTexture(renderer, MM_difficultyData.texture, NULL, &MM_difficultyData.rect);

    SDL_SetRenderViewport(renderer, NULL); // Change viewport back to fullscreen

    SDL_RenderTexture(renderer, MM_volumeData.texture, NULL, &MM_volumeData.rect);
    drawVolumeBar(&MM_volumeBtnData, &MM_volumeSliderData);

    return SDL_APP_CONTINUE;
}

//================================================================================ GAME LOOP

SDL_AppResult GameLogic_Input(void *appstate, SDL_Event *event)
{
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

SDL_AppResult GameLogic_Loop(void *appstate)
{
    Uint64 currentTick = SDL_GetTicks();

    tickDelta += (currentTick - previousTick);

    if (tickDelta >= tickRateMilliseconds)
    {
        tickDelta %= tickRateMilliseconds;

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

bool InitText(TextData *textData)
{
    if (textData->text == NULL || textData->font == NULL)
    {
        SDL_Log("falied to load text or font");
        return false;
    }

    TTF_SetFontSize(textData->font, textData->fontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(textData->font, textData->text, 0, textData->colour);

    if (textData->texture != NULL)
        SDL_DestroyTexture(textData->texture);

    if (!textSurface)
    {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return false;
    }

    if (textData->texture != NULL)
        SDL_DestroyTexture(textData->texture);

    textData->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    return true;
}

/* intended to be used "recursively" until the x coord reaches the negative texure width */
bool moveDifficultText(TextData *txtData, TextData *temp, float speed, float timeDelta, char *nextText)
{
    txtData->rect.x -= speed * timeDelta;

    if (temp != NULL)
    {
        temp->text = nextText;
        temp->font = txtData->font;
        temp->fontSize = txtData->fontSize;
        temp->colour = txtData->colour;
        InitText(temp);

        SDL_GetTextureSize(temp->texture, &temp->rect.w, &temp->rect.h);
        temp->rect.x = txtData->rect.x + txtData->rect.w;
        temp->rect.y = 0.0f;
    }
    // InitText(txtData);

    if (txtData->rect.x < -txtData->texture->w)
    {
        txtData->rect.x = 0.0f;
        txtData->text = nextText;

        InitText(txtData);
        return false;
    }

    return true;
}

bool CheckButtonState(ButtonData *btnData, SDL_Event *event)
{
    E_ButtonState btnState = NONE;
    if (SDL_PointInRectFloat(&mousePos, &btnData->textData.rect))
    {
        if (event == NULL || event->button.button != SDL_BUTTON_LEFT)
            btnState = HOVERING;
        else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            btnState = PRESSED;
        else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && btnData->currentState == PRESSED)
            btnState = RELEASED;
    }
    else if (btnData->currentState != PRESSED || event->type == SDL_EVENT_MOUSE_BUTTON_UP)
        btnState = NORMAL;

    if (btnState != NONE && ChangeButtonState(btnData, btnState))
    {
        btnData->textData.colour = getButtonTextColour(btnData);

        return true;
    }

    return false;
}

bool ChangeButtonState(ButtonData *btnData, E_ButtonState state)
{
    if (state == btnData->currentState)
        return false;

    btnData->previousState = btnData->currentState;
    btnData->currentState = state;

    return true;
}

SDL_Color getButtonTextColour(ButtonData *btnData)
{
    switch (btnData->currentState)
    {
    case PRESSED:
        return btnData->selectColour;
    case HOVERING:
        return btnData->highlightColour;
    case NORMAL:
    case RELEASED:
        return btnData->displayColour;
    }
}

void drawVolumeBar(ButtonData *btnData, SliderData *sliderData)
{
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &btnData->textData.rect);

    SDL_FRect vol;
    vol.x = sliderData->x;
    vol.y = sliderData->y;
    vol.w = sliderData->position - sliderData->x;
    vol.h = sliderData->height;

    SDL_SetRenderDrawColor(renderer, btnData->textData.colour.r, btnData->textData.colour.g, btnData->textData.colour.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &vol);
}

void changeVolume(SliderData *sliderData, float mouseX)
{
    if (mouseX < sliderData->x)
        sliderData->position = sliderData->x;
    else if (mouseX > (sliderData->x + sliderData->width))
        sliderData->position = sliderData->x + sliderData->width;
    else
        sliderData->position = mouseX;

    sliderData->normalisedPos = (sliderData->position - sliderData->x) / sliderData->width;
}
