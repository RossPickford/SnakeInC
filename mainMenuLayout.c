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

void InitMainMenuWidgets(Arena *arena)
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
        DI_BOX, DI_TEXT,  /* Folder Selection Button */
        DI_BOX,           /* Width Input Box */
        DI_BOX,           /* Height Input Box */
        DI_TEXT,          /* Width and Height 'X' */
        DI_BOX, DI_TEXT,  /* Create button */
        DI_BOX, DI_TEXT}; /* Cancel Button */

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

    // Assign button data to the
    for (int i = 0; i < 10; i++)
    {
        if (!(layoutButtons_groupPtr + i)->btnData)
            continue;

        (layoutButtons_groupPtr + i)->btnData->currentState = BSTATE_NORMAL;
        (layoutButtons_groupPtr + i)->btnData->previousState = BSTATE_NONE;
    }

    // Assigning New and Load layout display Text data.
    AssignText((Text *)MAIN_BUTTON_NEWLAYOUT->displayData->displayData, newLayoutTxt, &publicFont, 20.0f, &white);
    AssignText((Text *)MAIN_BUTTON_LOADLAYOUT->displayData->displayData, newLayoutTxt, &publicFont, 20.0f, &white);
    
    AssignBox((Box *)DLGBOX_DISPLAY_MAINBOX->displayData->displayData, 640.0f, 360.0f, &white, &black);
    AssignBox((Box *)DLGBOX_INPUT_NAMEFILE->displayData->displayData, 400.0f, 20.0f, &white, &black);
    AssignBox((Box *)DLGBOX_BUTTON_SELECTFOLDER->displayData->displayData, 20.0f, 20.0f, &white, &black);
    AssignText((((Text *)DLGBOX_BUTTON_SELECTFOLDER->displayData->displayData) + 1), newLayout_SelectFolderTxt, &publicFont, 20.0f, &white);
    AssignBox((Box *)DLGBOX_INPUT_SCREENWIDTH->displayData->displayData, 200.0f, 20.0f, &white, &black);
    AssignBox((Box *)DLGBOX_INPUT_SCREENHEIGHT->displayData->displayData, 200.0f, 20.0f, &white, &black);
    AssignText((Text *)DLGBOX_DISPLAY_WXH->displayData->displayData, newLayout_WxHText, &publicFont, 20.0f, &white);
    AssignBox((Box *)DLGBOX_BUTTON_CREATELAYOUT->displayData->displayData, 20.0f, 20.0f, &white, &black);
    

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
