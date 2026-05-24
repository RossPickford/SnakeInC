#include "mainMenuLayout.h"

#define LAYOUT_BUTTONS_GROUP_SIZE 2
#define DIALOGUE_BOX_GROUP_SIZE 8

#define MAIN_BUTTON_NEWLAYOUT layoutButtons_groupPtr
#define MAIN_BUTTON_LOADLAYOUT (layoutButtons_groupPtr + 1)

#define DLGBOX_DISPLAY_MAINBOX dialogueBox_groupPtr
#define DLGBOX_INPUT_NAMEFILE (dialogueBox_groupPtr + 1)
#define DLGBOX_BUTTON_SELECTFOLDER (dialogueBox_groupPtr + 2)
#define DLGBOX_INPUT_SCREENWIDTH (dialogueBox_groupPtr + 3)
#define DLGBOX_INPUT_SCREENHEIGHT (dialogueBox_groupPtr + 4)
#define DLGBOX_DISPLAY_WXH (dialogueBox_groupPtr + 5)
#define DLGBOX_BUTTON_CREATELAYOUT (dialogueBox_groupPtr + 6)
#define DLGBOX_BUTTON_CANCELLAYOUT (dialogueBox_groupPtr + 7)

#define M_NEWLAYOUT_X 0.0f
#define M_NEWLAYOUT_Y 0.0f

#define M_LOADLAYOUT_X 0.0f
#define M_LOADLAYOUT_Y MAIN_BUTTON_NEWLAYOUT->rect.h * 1.5f

#define DB_MAINBOX_X (width / DLGBOX_DISPLAY_MAINBOX->rect.w) / 2.0f
#define DB_MAINBOX_Y (height / DLGBOX_DISPLAY_MAINBOX->rect.h) / 2.0f

#define DB_NAMEFILE_X DB_MAINBOX_X + (DLGBOX_DISPLAY_MAINBOX->rect.w - DLGBOX_INPUT_NAMEFILE->rect.w) / 2.0f
#define DB_NAMEFILE_Y DB_MAINBOX_Y + (DLGBOX_DISPLAY_MAINBOX->rect.h / 3.0f)

#define DB_SELECTFOLDER_X DB_NAMEFILE_X + (DLGBOX_INPUT_NAMEFILE->rect.w * 1.2f)
#define DB_SELECTFOLDER_Y DB_NAMEFILE_Y

#define DB_SCREENWIDTH_X DB_MAINBOX_X + (DLGBOX_DISPLAY_MAINBOX->rect.w - (DLGBOX_INPUT_SCREENWIDTH->rect.w + DLGBOX_INPUT_SCREENHEIGHT->rect.w + DLGBOX_DISPLAY_WXH->rect.w + 10.0f)) / 2.0f
#define DB_SCREENWIDTH_Y DB_NAMEFILE_Y + (DLGBOX_DISPLAY_MAINBOX->rect.h / 3.0f)

#define DB_SCREENHEIGHT_X DB_WXH_X + 5.0f
#define DB_SCREENHEIGHT_Y DB_SCREENWIDTH_Y

#define DB_WXH_X DB_SCREENWIDTH_X + 5.0f
#define DB_WXH_Y DB_SCREENWIDTH_Y

#define DB_CREATELAYOUT_X DB_MAINBOX_X + (DLGBOX_DISPLAY_MAINBOX->rect.w - (DLGBOX_BUTTON_CREATELAYOUT->rect.w + DLGBOX_BUTTON_CANCELLAYOUT->rect.w + 10.0f)) / 2.0f
#define DB_CREATELAYOUT_Y DB_SCREENWIDTH_Y + (DLGBOX_DISPLAY_MAINBOX->rect.h / 3.0f)

#define DB_CANCELLAYOUT_X DB_CREATELAYOUT_X + DLGBOX_BUTTON_CREATELAYOUT->rect.w + 10.0f
#define DB_CANCELLAYOUT_Y DB_CREATELAYOUT_Y

static SDL_Color black = {0, 0, 0, 255};
static SDL_Color white = {255, 255, 255, 255};
static SDL_Color red = {255, 0, 0, 255};
static SDL_Color green = {0, 255, 0, 255};
static SDL_Color blue = {0, 0, 255, 255};

static char *publicFont = "./fonts/PublicPixel.ttf";

// First widgets to appear =================
TextButton newLayoutBtn;
char *newLayoutTxt = "New Layout";

TextButton loadLayoutBtn;
char *loadLayoutTxt = "Load Layout...";

// =========================================

// New Layout dialogue box widgets =========
BoxDisplay newLayout_DialogueBox;

InputTextBox newLayout_NameInput;

char *newLayout_SelectFolderTxt = "...";

InputTextBox newLayout_widthInput;

InputTextBox newLayout_heightInput;

TextDisplay newLayout_WxH;
static char newLayout_WxHText = 'X';

BoxButton newLayout_createButtonBox;
static char *newLayout_createText = "create";

BoxButton newLayout_cancelButtonBox;
static char *newLayout_cancelText = "cancel";

//===============================================

UI_Element *layoutButtons_groupPtr, *dialogueBox_groupPtr;
UI_Element *E_newLayout, *E_loadLayout;

UI_Element *E_dlgue_box, *E_dlgue_inputBox, *E_dlgue_slctFldr, *E_wdthInpt, *E_hghtInpt, *E_wdthxHght, *E_crtBtn, *E_cnclBtn;

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

void LinkAllocateDisplayToElement(Arena *arena, displayType *display, displayType_ID id)
{
    size_t size = getTypeFromID(id);

    display = (displayType *)ArenaAlloc(arena, sizeof(displayType));
    display->id = DI_TEXT;
    display->displayData = ArenaAlloc(arena, size);
}

void AssignText(Text *txt, char *str, char **fontFile, float fontSize, SDL_Color *colour)
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
    bx->fillColor = fillColour;
}

float getTextHeight(Text *txt, SDL_Renderer *renderer)
{
    float height = 0;
    TTF_Font *font = TTF_OpenFont(*txt->fontFile, txt->fontSize);
    SDL_Surface *tempSurf = TTF_RenderText_Blended(font, *txt->text, 0, white);
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
    TTF_Font *font = TTF_OpenFont(*txt->fontFile, txt->fontSize);
    SDL_Surface *tempSurf = TTF_RenderText_Blended(font, txt->text, 0, white);
    SDL_Texture *tempText = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_DestroySurface(tempSurf);
    SDL_GetTextureSize(tempText, &width, NULL);
    SDL_DestroyTexture(tempText);
    TTF_CloseFont(font);
    return width;
}

SDL_Surface *createTextSurface(Text *txt)
{
    TTF_Font *font = TTF_OpenFont(*txt->fontFile, txt->fontSize);
    SDL_Surface *surf = TTF_RenderText_Blended(font, txt->text, 0, *txt->colour);
    TTF_CloseFont(font);
    return surf;
}

SDL_Surface *createBoxSurface(Box *bx)
{
    SDL_Surface *surf = SDL_CreateSurface(bx->width, bx->height, SDL_PIXELFORMAT_ABGR128_FLOAT);
    Uint32 edgeColour = SDL_MapRGBA(SDL_PIXELFORMAT_ABGR128_FLOAT, NULL, bx->edgeColour->r, bx->edgeColour->g, bx->edgeColour->b, bx->edgeColour->a);
    SDL_FillSurfaceRect(surf, NULL, edgeColour);

    SDL_Rect rect = {1, 1, (bx->width - 2), (bx->height - 2)};
    Uint32 fillColour = SDL_MapRGBA(SDL_PIXELFORMAT_ABGR128_FLOAT, NULL, bx->fillColor->r, bx->fillColor->g, bx->fillColor->b, bx->fillColor->a);
    SDL_FillSurfaceRect(surf, &rect, fillColour);

    return surf;
}

void createUITexture(UI_Element *ui, SDL_Renderer *renderer, float x, float y)
{
    ui->rect.x = x;
    ui->rect.y = y;

    SDL_Surface *surfs[ui->displayCount];

    size_t offset = 0;
    for (size_t i = 0; i < ui->displayCount; i++)
    {
        switch ((ui->displayData + i)->id)
        {
        case DI_TEXT:
            surfs[i] = createTextSurface((Text *)(ui->displayData + i)->displayData);
            offset += sizeof(Text);
            break;
        case DI_BOX:
            surfs[i] = createBoxSurface((Box *)(ui->displayData + i)->displayData);
            offset += sizeof(Box);
            break;
        case DI_IMAGE:
            break;
        }
    }

    SDL_UnlockSurface(*surfs);
    for (size_t i = 1; i < ui->displayCount; i++)
    {
        SDL_UnlockSurface(surfs[i]);
        SDL_BlitSurface(surfs[i], NULL, NULL, *surfs);
        SDL_DestroySurface(surfs[i]);
    }
    SDL_LockSurface(*surfs);

    ui->texture = SDL_CreateTextureFromSurface(renderer, *surfs);
    SDL_DestroySurface(*surfs);
    SDL_GetTextureSize(ui->texture, &ui->rect.w, &ui->rect.h);
}

void InitMainMenuWidgets(Arena *arena, SDL_Renderer *renderer, size_t width, size_t height)
{
    layoutButtons_groupPtr = (UI_Element *)ArenaAlloc(arena, sizeof(UI_Element) * LAYOUT_BUTTONS_GROUP_SIZE);
    // 0 - new layout button
    // 1 - load layout button

    dialogueBox_groupPtr = (UI_Element *)ArenaAlloc(arena, sizeof(UI_Element) * DIALOGUE_BOX_GROUP_SIZE);
    // 0 - main encapsulating box
    // 1 - name input box
    // 2 - select folder button
    // 3 - width input box
    // 4 - height input box
    // 5 - width by height 'x' text
    // 6 - create layout button
    // 7 - cancel layout buttton

    layoutButtons_groupPtr->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData));       // Create layout button data allocation
    (layoutButtons_groupPtr + 1)->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // load layout button data allocation

    DLGBOX_BUTTON_SELECTFOLDER->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // select folder button
    DLGBOX_BUTTON_CREATELAYOUT->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // create layout button
    DLGBOX_BUTTON_CANCELLAYOUT->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // cancel dialogue box button

    LinkAllocateDisplayToElement(arena, MAIN_BUTTON_NEWLAYOUT->displayData, DI_TEXT); // allocating text data type for new layout button
    MAIN_BUTTON_NEWLAYOUT->displayCount = 1;
    LinkAllocateDisplayToElement(arena, MAIN_BUTTON_LOADLAYOUT->displayData, DI_TEXT); // allocating text data type for load layout button
    MAIN_BUTTON_LOADLAYOUT->displayCount = 1;

    displayType_ID ids[] = {
        DI_BOX,           /* Main Dialogue Box */
        DI_BOX,           /* File Name Input Box */
        DI_TEXT, DI_BOX,  /* Folder Selection Button */
        DI_BOX,           /* Width Input Box */
        DI_BOX,           /* Height Input Box */
        DI_TEXT,          /* Width and Height 'X' */
        DI_TEXT, DI_BOX,  /* Create button */
        DI_TEXT, DI_BOX}; /* Cancel Button */

    size_t idSize[] = {
        1,  /* Main Dialogue Box */
        1,  /* File Name Input Box */
        2,  /* Folder Selection Button */
        1,  /* Width Input Box */
        1,  /* Height Input Box */
        1,  /* Width and Height 'X' */
        2,  /* Create button */
        2}; /* Cancel Button */

    // Allocating the display data type for each UI Element in the dialogue box
    for (size_t i = 0, j = 0; i < DIALOGUE_BOX_GROUP_SIZE; i++)
    {
        size_t k = 0;
        (dialogueBox_groupPtr + i)->displayCount = idSize[i];
        while (k < idSize[i])
            LinkAllocateDisplayToElement(arena, (dialogueBox_groupPtr + i)->displayData, ids[j + k++]);
        j += k;
    }

    // Assign button data to the coresponding buttons
    for (int i = 0; i < 10; i++)
    {
        if (!(layoutButtons_groupPtr + i)->btnData)
            continue;

        (layoutButtons_groupPtr + i)->btnData->currentState = BSTATE_NORMAL;
        (layoutButtons_groupPtr + i)->btnData->previousState = BSTATE_NONE;
    }

    // Assigning display data for each element in the same order they were allocated

    // New and Load layout buttons
    AssignText((Text *)MAIN_BUTTON_NEWLAYOUT->displayData->displayData, newLayoutTxt, &publicFont, 20.0f, &white);
    AssignText((Text *)MAIN_BUTTON_LOADLAYOUT->displayData->displayData, loadLayoutTxt, &publicFont, 20.0f, &white);

    // Dialogue box main encapsulating box
    AssignBox((Box *)DLGBOX_DISPLAY_MAINBOX->displayData->displayData, width / 3.0f, height / 3.0f, &white, &black);

    // Dlg box name file input box
    AssignBox((Box *)DLGBOX_INPUT_NAMEFILE->displayData->displayData, width / 5.0f, 20.0f, &white, &black);

    // Dlg box select folder button
    Text *slctFldrText = (Text *)DLGBOX_BUTTON_SELECTFOLDER->displayData->displayData;
    AssignText(slctFldrText, newLayout_SelectFolderTxt, &publicFont, 20.0f, &white);
    float bxWidth = getTextWidth(slctFldrText, renderer);
    float bxHeight = getTextHeight(slctFldrText, renderer);
    AssignBox((Box *)(slctFldrText + 1), bxWidth, bxHeight, &white, &black);

    // Dlg box input screen width and height
    AssignBox((Box *)DLGBOX_INPUT_SCREENWIDTH->displayData->displayData, 200.0f, 20.0f, &white, &black);
    AssignBox((Box *)DLGBOX_INPUT_SCREENHEIGHT->displayData->displayData, 200.0f, 20.0f, &white, &black);
    AssignText((Text *)DLGBOX_DISPLAY_WXH->displayData->displayData, newLayout_WxHText, &publicFont, 20.0f, &white);

    // Dlg box create layout button
    Text *creatBtnText = (Text *)DLGBOX_BUTTON_CREATELAYOUT->displayData->displayData;
    AssignText(creatBtnText, newLayout_createText, &publicFont, 20.0f, &white);
    bxWidth = getTextFontWidth(creatBtnText, renderer);
    bxHeight = getTextHeight(creatBtnText, renderer);
    AssignBox((Box *)(creatBtnText + 1), width, height, &white, &black);

    // Dlg box cancel layout button
    Text *cnclBtnText = (Text *)DLGBOX_BUTTON_CANCELLAYOUT->displayData->displayData;
    AssignText(cnclBtnText, newLayout_cancelText, &publicFont, 20.0f, &white);
    width = getTextWidth(cnclBtnText, renderer);
    height = getTextHeight(cnclBtnText, renderer);
    AssignBox((Box *)(cnclBtnText + 1), width, height, &white, &black);

    createUITexture(MAIN_BUTTON_NEWLAYOUT, renderer, M_NEWLAYOUT_X, M_NEWLAYOUT_Y);
    createUITexture(MAIN_BUTTON_LOADLAYOUT, renderer, M_LOADLAYOUT_X, M_LOADLAYOUT_Y);
    createUITexture(DLGBOX_DISPLAY_MAINBOX, renderer, DB_MAINBOX_X, DB_MAINBOX_Y);
    createUITexture(DLGBOX_INPUT_NAMEFILE, renderer, DB_NAMEFILE_X, DB_NAMEFILE_Y);
    createUITexture(DLGBOX_BUTTON_SELECTFOLDER, renderer, DB_SELECTFOLDER_X, DB_SELECTFOLDER_Y);
    createUITexture(DLGBOX_INPUT_SCREENWIDTH, renderer, DB_SCREENWIDTH_X, DB_SCREENWIDTH_Y);
    createUITexture(DLGBOX_INPUT_SCREENHEIGHT, renderer, DB_SCREENHEIGHT_X, DB_SCREENHEIGHT_Y);
    createUITexture(DLGBOX_DISPLAY_WXH, renderer, DB_WXH_X, DB_WXH_Y);
    createUITexture(DLGBOX_BUTTON_CREATELAYOUT, renderer, DB_CREATELAYOUT_X, DB_CREATELAYOUT_Y);
    createUITexture(DLGBOX_BUTTON_CANCELLAYOUT, renderer, DB_CANCELLAYOUT_X, DB_CANCELLAYOUT_Y);
}

/*
- new layout button <-
- load layout button <-
- new layout dialogue box
- -> input box for name <-
- -> Box button for folder selection
- -> input box for width
- -> input box for height
- -> image or text for the 'x' in 'width x height'
- -> create button
- -> cancel button
*/
