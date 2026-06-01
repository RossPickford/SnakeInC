#include "ui_types.h"

size_t getTypeFromID(displayType_ID id)
{
    switch (id)
    {
    case DI_BOX:
        return sizeof(Box);
    case DI_IMAGE:
        return sizeof(Image);
    case DI_TEXT:
        return sizeof(Text);
    default:
        return 0;
    }
}

void LinkAllocateDisplayToElement(Arena *arena, UI_Element *elmnt, displayType_ID *id, size_t displayCount)
{
    elmnt->displayCount = displayCount;
    elmnt->displayData = (displayType *)arenaAlloc(arena, sizeof(displayType) * displayCount);

    for (size_t i = 0; i < displayCount; i++)
    {
        size_t size = getTypeFromID(*(id + i));
        (elmnt->displayData + i)->id = *(id + i);
        (elmnt->displayData + i)->displayData = arenaAlloc(arena, size);
    }
}

void AssignText(Text *txt, char *str, char *fontFile, float fontSize, SDL_Color *colour)
{
    txt->text = str;
    txt->fontFile = fontFile;
    txt->fontSize = fontSize;
    txt->colour = colour;
}

void AssignBox(Box *bx, float width, float height, SDL_Color *edgeColour, SDL_Color *fillColour)
{
    bx->width = width;
    bx->height = height;
    bx->edgeColour = edgeColour;
    bx->fillColour = fillColour;
}

float getTextHeight(Text *txt, SDL_Renderer *renderer)
{
    float height = 0;
    TTF_Font *font = TTF_OpenFont(txt->fontFile, txt->fontSize);
    SDL_Color tempCol = {0, 0, 0, 0};
    SDL_Surface *tempSurf = TTF_RenderText_Blended(font, txt->text, 0, tempCol);
    SDL_Texture *tempText = SDL_CreateTextureFromSurface(renderer, tempSurf);

    SDL_DestroySurface(tempSurf);
    SDL_GetTextureSize(tempText, NULL, &height);
    SDL_DestroyTexture(tempText);
    TTF_CloseFont(font);

    return height;
}

float getTextWidth(Text *txt, SDL_Renderer *renderer)
{
    float width = 0;
    TTF_Font *font = TTF_OpenFont(txt->fontFile, txt->fontSize);
    SDL_Color tempCol = {0, 0, 0, 0};
    SDL_Surface *tempSurf = TTF_RenderText_Blended(font, txt->text, 0, tempCol);
    SDL_Texture *tempText = SDL_CreateTextureFromSurface(renderer, tempSurf);
    
    SDL_GetTextureSize(tempText, &width, NULL);
    SDL_DestroySurface(tempSurf);
    SDL_DestroyTexture(tempText);
    TTF_CloseFont(font);
    
    return width;
}

SDL_Surface *createTextSurface(Text *txt)
{
    TTF_Font *font = TTF_OpenFont(txt->fontFile, txt->fontSize);
    SDL_Surface *surf = TTF_RenderText_Blended(font, txt->text, 0, *txt->colour);
    TTF_CloseFont(font);
    return surf;
}

SDL_Surface *createBoxSurface(Box *bx)
{
    SDL_Surface *surf = SDL_CreateSurface(bx->width, bx->height, SDL_PIXELFORMAT_RGBA8888);
    Uint32 edgeColour = SDL_MapRGBA(SDL_GetPixelFormatDetails(surf->format), NULL, bx->edgeColour->r, bx->edgeColour->g, bx->edgeColour->b, bx->edgeColour->a);
    SDL_FillSurfaceRect(surf, NULL, edgeColour);

    SDL_Rect rect = {1, 1, (bx->width - 2), (bx->height - 2)};
    Uint32 fillColour = SDL_MapRGBA(SDL_GetPixelFormatDetails(surf->format), NULL, bx->fillColour->r, bx->fillColour->g, bx->fillColour->b, bx->fillColour->a);
    SDL_FillSurfaceRect(surf, &rect, fillColour);

    return surf;
}

void createUITexture(UI_Element *ui, SDL_Renderer *renderer)
{
    SDL_Surface *surfs[ui->displayCount];

    for (size_t i = 0; i < ui->displayCount; i++)
    {
        switch ((ui->displayData + i)->id)
        {
        case DI_TEXT:

            surfs[i] = createTextSurface((Text *)(ui->displayData + i)->displayData);
            break;
        case DI_BOX:
            surfs[i] = createBoxSurface((Box *)(ui->displayData + i)->displayData);
            break;
        case DI_IMAGE:
            break;
        }
    }

    SDL_UnlockSurface(*surfs);
    for (size_t i = 1; i < ui->displayCount; i++)
    {
        SDL_UnlockSurface(surfs[i]);
        SDL_BlitSurface(surfs[i], NULL, *surfs, NULL);
        SDL_DestroySurface(surfs[i]);
    }
    SDL_LockSurface(*surfs);

    ui->texture = SDL_CreateTextureFromSurface(renderer, *surfs);
    SDL_DestroySurface(*surfs);
    SDL_GetTextureSize(ui->texture, &ui->rect.w, &ui->rect.h);
}

void setUITextureCoords(UI_Element *ui, float x, float y)
{
    // SDL_Log("x: %f, y: %f", x, y);
    ui->rect.x = x;
    ui->rect.y = y;
}