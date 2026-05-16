
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#pragma once

typedef enum UIType
{
    ITYPE_BUTTON_TEXT,
    ITYPE_BUTTON_IMAGE,
    ITYPE_BUTTON_BOX,
    ITYPE_DISPLAY_TEXT,
    ITYPE_DISPLAY_BOX,
    ITYPE_INPUT_TEXTBOX,
} UIType;

typedef enum InputAction
{
    IACTION_NONE,
    IACTION_SHOW_CHILDREN,

} InputAction;

typedef enum ButtonState
{
    BSTATE_PRESSED,
    BSTATE_RELEASED,
    BSTATE_HOVERING,
    BSTATE_NORMAL,
    BSTATE_NONE,
} ButtonState;

typedef struct UIType_ID
{
    UIType type;
    InputAction action;
    uint16_t childrenCount;
    void *children;
    void *data;
} UIType_ID;

typedef struct TextDisplay
{
    char *text;
    SDL_Texture *texture;
    TTF_Font *font;
    float fontSize;
    SDL_Color colour;
    SDL_FRect rect;
} TextDisplay;

typedef struct TextButton
{
    TextDisplay textData;
    SDL_Color displayColour;
    SDL_Color highlightColour;
    SDL_Color selectColour;
    ButtonState currentState;
    ButtonState previousState;
} TextButton;

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
} InputTextBox;

typedef struct
{
    SDL_FRect box;
    SDL_Texture *texture;
    SDL_Color *edgeColour;
    SDL_Color *fillColor;
} BoxDisplay;

/***
 * edgeColours & fillColours - requires an array of 3 SDL_Colors - the first is display colour, the second is highlight colour, and the third is selected colour.
 */
typedef struct
{
    BoxDisplay boxDisplay;
    SDL_Color edgeColours[3];
    SDL_Color fillColours[3];
    ButtonState currentState;
    ButtonState previousSate;
} BoxButton;

typedef struct
{

} ImageDisplay;

typedef struct
{
    /* data */
} ImageButton;

typedef struct ButtonData
{
    ButtonState currentState;
    ButtonState previousState;
    SDL_Color **colours;
} ButtonData;

typedef enum displayType_ID
{
    DI_TEXT,
    DI_BOX,
    DI_IMAGE,
} displayType_ID;

typedef struct DisplayType
{
    displayType_ID id;
    void *displayData;

} displayType;

typedef struct UIElement
{
    SDL_Texture *texture;
    SDL_FRect rect;
    ButtonData *btnData;
    size_t displayCount;
    displayType *displayData;
    // SDL_Color **displayColours;
} UIElement;

// store display data and colour data into an arena

typedef struct Arena
{
    void *start;
    void *currentPos;
    // size_t offset;
} Arena;

void *ArenaAlloc(Arena *arena, size_t size)
{
    void *current = arena->currentPos;

    arena->currentPos = (char *)arena->currentPos + (size - 1);

    return current;
}

TextDisplay *createText(Arena *arena, SDL_Color colour, TTF_Font *font, size_t fontSize)
{
    TextDisplay *text = (TextDisplay *)ArenaAlloc(arena, sizeof(TextDisplay));
    text->colour = colour;
    text->font = font;
    text->fontSize = fontSize;
}
