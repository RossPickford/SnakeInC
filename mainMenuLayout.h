#pragma once

#include "ui_types.h"

#define MAINMENU_ELEMENT_COUNT 10
#define LAYOUT_BUTTONS_GROUP_SIZE 2
#define DIALOGUE_BOX_GROUP_SIZE 8

extern UI_Element *layoutButtons_groupPtr, *dialogueBox_groupPtr;

bool initMainMenuWidgets(Arena *arena, SDL_Renderer *renderer, size_t width, size_t height);