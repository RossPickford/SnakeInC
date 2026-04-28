#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define MAX_CHAR_INPUT 20

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
    bool selected;

} InputTextBoxData;

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

    if ((textData->texture = SDL_CreateTextureFromSurface(renderer, textSurface)) == NULL)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return false;
    }

    SDL_DestroySurface(textSurface);

    if (!SDL_GetTextureSize(textData->texture, &textData->rect.w, &textData->rect.h))
    {
        SDL_Log("Failed to get texture size: %s", SDL_GetError());
        return false;
    }

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

/**
 * This checks and will update the current stat of a button based on the given mouse position.
 *
 * This will return true if the button has changed state and false if the state has remained the same.
 *
 * Use this in an if statement to add in logic changes if the change of state was true
 */
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
void AssignTextData(TextData *txtData, char *txt, TTF_Font *font, float fontSize, SDL_Color colour)
{
    txtData->text = txt;
    txtData->font = font;
    txtData->fontSize = fontSize;
    txtData->colour = colour;
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

bool SetTextPosition(TextData *txtData, float x, float y)
{
    txtData->rect.x = x;
    txtData->rect.y = y;

    return true;
}

bool InitInputTextBox(SDL_Renderer *renderer, InputTextBoxData *txtBx, int maxInputLength, TTF_Font *font, float fontSize, SDL_Color *colour)
{
    txtBx->maxInputLength = maxInputLength;
    txtBx->font = font;
    txtBx->fontSize = fontSize;
    txtBx->colour = colour;
    txtBx->input = (char *)SDL_malloc(sizeof(char) * maxInputLength);
    *txtBx->input = '\0';
    txtBx->currentPos = 0;

    SDL_Surface *surf = TTF_RenderText_Solid(font, txtBx->input, maxInputLength, *colour);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    SDL_GetTextureSize(tex, &txtBx->cursor.w, &txtBx->cursor.h);
    SDL_DestroyTexture(tex);
}

bool RenderInputTextBox(InputTextBoxData *txtBx, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, txtBx->colour->r, txtBx->colour->g, txtBx->colour->b, txtBx->colour->a);
    SDL_RenderRect(renderer, &txtBx->textBox);

    SDL_Rect viewPort;

    viewPort.x = txtBx->textBox.x;
    viewPort.y = txtBx->textBox.y;
    viewPort.w = txtBx->textBox.w;
    viewPort.h = txtBx->textBox.h;

    SDL_SetRenderViewport(renderer, &viewPort);

    SDL_Surface *displaytextSurf = TTF_RenderText_Solid(txtBx->font, txtBx->input, txtBx->currentPos, *txtBx->colour);
    SDL_Texture *displayTexture = SDL_CreateTextureFromSurface(renderer, displaytextSurf);
    SDL_DestroySurface(displaytextSurf);

    SDL_FRect rect;
    rect.x = rect.y = 0.0f;
    SDL_GetTextureSize(displayTexture, &rect.w, &rect.h);
    SDL_RenderTexture(renderer, displayTexture, NULL, &rect);
    SDL_DestroyTexture(displayTexture);

    SDL_Surface *surf = TTF_RenderText_Solid(txtBx->font, " ", 1, *txtBx->colour);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    SDL_GetTextureSize(texture, &txtBx->cursor.w, &txtBx->cursor.h);
    SDL_DestroyTexture(texture);

    txtBx->cursor.x = rect.w;
    txtBx->cursor.y = rect.y;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &txtBx->cursor);

    SDL_SetRenderViewport(renderer, NULL);

    return true;
}

void SizeTextBoxToMaxCharInput(InputTextBoxData *txtBx, SDL_Renderer *renderer)
{
    SDL_Surface *surf = TTF_RenderText_Solid(txtBx->font, txtBx->input, txtBx->maxInputLength, *txtBx->colour);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_DestroySurface(surf);

    SDL_GetTextureSize(text, &txtBx->textBox.w, &txtBx->textBox.h);
    SDL_DestroyTexture(text);
}

bool InsertCharacter(InputTextBoxData *txtBx, const char *input)
{
    if (txtBx->currentPos >= txtBx->maxInputLength)
        return false;

    int len = (int)SDL_strlen(input);
    char *copy = (char *)SDL_malloc(len);

    SDL_strlcpy(copy, input, (len + 1));

    if (SDL_strcmp(copy, "Space") == 0)
    {
        *copy = ' ';
        *(copy + 1) = '\0';
    }

    if (*(copy + 1) != '\0')
    {
        SDL_free(copy);
        return false;
    }

    *(txtBx->input + txtBx->currentPos++) = *copy;
    SDL_free(copy);

    return true;
}
