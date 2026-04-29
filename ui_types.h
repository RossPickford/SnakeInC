
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#pragma once

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
} ButtonData;

typedef struct
{
    char *input;
    int maxInputLength;
    int currentPos;
    TTF_Font *font;
    float fontSize;
    SDL_FRect textBox;
    SDL_FRect cursor;
    SDL_Color *colour;
} InputTextBoxData;

