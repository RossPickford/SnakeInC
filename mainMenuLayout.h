#pragma once

#include "ui_types.h"
#include "arena.h"

#define MAINMENU_ELEMENT_COUNT 10

bool InitMainMenuWidgets(Arena *arena, SDL_Renderer *renderer, size_t width, size_t height);
UI_Element *getMainMenuElements();