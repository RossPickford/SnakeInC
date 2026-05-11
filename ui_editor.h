#include "ui_types.h"

#pragma once

bool UpdateText(TextDisplay *textData, SDL_Renderer *renderer);
bool ChangeButtonState(TextButton *btnData, ButtonState state);
SDL_Color GetButtonTextColour(TextButton *btnData);
bool CheckButtonState(TextButton *btnData, SDL_Event *event, SDL_FPoint *mousePosPtr);
void AssignTextData(TextDisplay *txtData, char *txt, TTF_Font *font, float fontSize, SDL_Color colour);
void AssignButtonData(TextButton *btn, SDL_Color display, SDL_Color highlight, SDL_Color select, ButtonState curState, ButtonState prevState);
bool SetTextPosition(TextDisplay *txtData, float x, float y);
bool InitInputTextBox(SDL_Renderer *renderer, InputTextBox *txtBx, int maxInputLength, TTF_Font *font, float fontSize, SDL_Color *colour);
bool RenderInputTextBox(InputTextBox *txtBx, SDL_Renderer *renderer);
void SizeTextBoxToMaxCharInput(InputTextBox *txtBx, SDL_Renderer *renderer);
bool InsertCharacter(InputTextBox *txtBx, const char *input);