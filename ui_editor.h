#include "ui_types.h"

bool UpdateText(TextData *textData, SDL_Renderer *renderer);

bool ChangeButtonState(ButtonData *btnData, E_ButtonState state);

SDL_Color GetButtonTextColour(ButtonData *btnData);

/**
 * This checks and will update the current stat of a button based on the given mouse position.
 *
 * This will return true if the button has changed state and false if the state has remained the same.
 *
 * Use this in an if statement to add in logic changes if the change of state was true
 */
bool CheckButtonState(ButtonData *btnData, SDL_Event *event, SDL_FPoint *mousePosPtr);

/**
 * Assigns data to the parameters of the Text Data Struct
 *
 * This DOES NOT assign the pointer texture - call UpdateText() after calling this function to create
 * and assign a texture and it's corresponding pointer to the text data struct.
 */
void AssignTextData(TextData *txtData, char *txt, TTF_Font *font, float fontSize, SDL_Color colour);

/**
 * Assigns state data to the button data.
 *
 * This does not need to be called before calling AssignTextData(), however it would be advised for clarity
 */
void AssignButtonData(ButtonData *btn, SDL_Color display, SDL_Color highlight, SDL_Color select, E_ButtonState curState, E_ButtonState prevState);

bool SetTextPosition(TextData *txtData, float x, float y);

bool InitInputTextBox(SDL_Renderer *renderer, InputTextBoxData *txtBx, int maxInputLength, TTF_Font *font, float fontSize, SDL_Color *colour);

bool RenderInputTextBox(InputTextBoxData *txtBx, SDL_Renderer *renderer);

void SizeTextBoxToMaxCharInput(InputTextBoxData *txtBx, SDL_Renderer *renderer);

bool InsertCharacter(InputTextBoxData *txtBx, const char *input);