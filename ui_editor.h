#include "ui_types.h"

#pragma once

bool UpdateText(TextData *textData, SDL_Renderer *renderer);
bool ChangeButtonState(ButtonData *btnData, E_ButtonState state);
SDL_Color GetButtonTextColour(ButtonData *btnData);
bool CheckButtonState(ButtonData *btnData, SDL_Event *event, SDL_FPoint *mousePosPtr);
void AssignTextData(TextData *txtData, char *txt, TTF_Font *font, float fontSize, SDL_Color colour);
void AssignButtonData(ButtonData *btn, SDL_Color display, SDL_Color highlight, SDL_Color select, E_ButtonState curState, E_ButtonState prevState);
bool SetTextPosition(TextData *txtData, float x, float y);
bool InitInputTextBox(SDL_Renderer *renderer, InputTextBoxData *txtBx, int maxInputLength, TTF_Font *font, float fontSize, SDL_Color *colour);
bool RenderInputTextBox(InputTextBoxData *txtBx, SDL_Renderer *renderer);
void SizeTextBoxToMaxCharInput(InputTextBoxData *txtBx, SDL_Renderer *renderer);
bool InsertCharacter(InputTextBoxData *txtBx, const char *input);