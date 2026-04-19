#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

bool UpdateText(TextData *textData, SDL_Renderer *renderer)
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

bool ChangeButtonState(ButtonData *btnData, E_ButtonState state)
{
    if (state == btnData->currentState)
        return false;

    btnData->previousState = btnData->currentState;
    btnData->currentState = state;

    return true;
}

SDL_Color GetButtonTextColour(ButtonData *btnData)
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

bool CheckButtonState(ButtonData *btnData, SDL_Event *event, SDL_FPoint *mousePos)
{
    E_ButtonState btnState = NONE;
    if (SDL_PointInRectFloat(mousePos, &btnData->textData.rect))
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
        btnData->textData.colour = GetButtonTextColour(btnData);

        return true;
    }

    return false;
}

/**
 * Assigns data to the parameters of the Text Data Struct
 *
 * This DOES NOT assign the pointer texture - call UpdateText() after calling this function to create
 * and assign a texture and it's corresponding pointer to the text data struct.
 */
void AssignTextData(TextData *txtData, char *txt, TTF_Font *font, float fontSize, SDL_Color colour /*, SDL_FRect rect*/)
{
    txtData->text = txt;
    txtData->font = font;
    txtData->fontSize = fontSize;
    txtData->colour = colour;
    // txtData->rect = rect;
}

/**
 * Assigns state data to the button data.
 *
 * This does not need to be called before calling AssignTextData(), however it would be advised for clarity
 */
void AssignButtonData(ButtonData *btn, SDL_Color display, SDL_Color highlight, SDL_Color select, E_ButtonState curState, E_ButtonState prevState)
{
    btn->displayColour = display;
    btn->highlightColour = highlight;
    btn->selectColour = select;
    btn->currentState = curState;
    btn->previousState = prevState;
}