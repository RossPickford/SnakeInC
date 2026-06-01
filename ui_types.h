#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "arena.h"

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
    SDL_Color *fillColour;
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

typedef struct UI_Element
{
    SDL_Texture *texture;
    SDL_FRect rect;
    size_t displayCount;
    displayType *displayData;
    ButtonData *btnData;
    // SDL_Color **displayColours;
} UI_Element;

typedef struct Text
{
    char *text;
    char *fontFile;
    float fontSize;
    SDL_Color *colour;
} Text;

typedef struct Box
{
    float width;
    float height;
    SDL_Color *edgeColour;
    SDL_Color *fillColour;
} Box;

typedef struct Image
{

} Image;

size_t getTypeFromID(displayType_ID id);

void LinkAllocateDisplayToElement(Arena *arena, UI_Element *elmnt, displayType_ID *id, size_t displayCount);

void AssignText(Text *txt, char *str, char *fontFile, float fontSize, SDL_Color *colour);

void AssignBox(Box *bx, float width, float height, SDL_Color *edgeColour, SDL_Color *fillColour);

float getTextHeight(Text *txt, SDL_Renderer *renderer);

float getTextWidth(Text *txt, SDL_Renderer *renderer);

SDL_Surface *createTextSurface(Text *txt);

SDL_Surface *createBoxSurface(Box *bx);

void createUITexture(UI_Element *ui, SDL_Renderer *renderer);

void setUITextureCoords(UI_Element *ui, float x, float y);