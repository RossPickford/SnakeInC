#include "mainMenuLayout.h"

#define LAYOUT_BUTTONS_GROUP_SIZE 2
#define DIALOGUE_BOX_GROUP_SIZE 8

// First widgets to appear =================
TextButton newLayoutBtn;
char *newLayoutTxt = "New Layout";

TextButton loadLayoutBtn;
char *loadLayoutTxt = "Load Layout...";

// =========================================

// New Layout dialogue box widgets =========
BoxDisplay newLayout_DialogueBox;

InputTextBox newLayout_NameInput;

ImageButton newLayout_SelectFolder;

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
        return sizeof(BoxDisplay);
    case DI_IMAGE:
        return sizeof(ImageDisplay);
    case DI_TEXT:
        return sizeof(TextDisplay);
    default:
        return 0;
    }
}

void addDisplayToElement(Arena *arena, displayType *display, displayType_ID id)
{
    size_t size = getTypeFromID(id);

    display = (displayType *)ArenaAlloc(arena, sizeof(displayType));
    display->id = DI_TEXT;
    display->displayData = ArenaAlloc(arena, size);
}

void InitMainMenuWidgets(Arena *arena)
{
    layoutButtons_groupPtr = (UI_Element *)ArenaAlloc(arena, sizeof(UI_Element) * LAYOUT_BUTTONS_GROUP_SIZE);
    // 0 - create layout button
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

    (dialogueBox_groupPtr + 2)->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // select folder button
    (dialogueBox_groupPtr + 6)->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // create layout button
    (dialogueBox_groupPtr + 7)->btnData = (ButtonData *)ArenaAlloc(arena, sizeof(ButtonData)); // cancel dialogue box button

    addDisplayToElement(arena, layoutButtons_groupPtr->displayData, DI_TEXT); // allocating text data type for create layout button
    layoutButtons_groupPtr->displayCount = 1;
    addDisplayToElement(arena, (layoutButtons_groupPtr + 1)->displayData, DI_TEXT); // allocating text data type for load layout button
    (layoutButtons_groupPtr + 1)->displayCount = 1;

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
            addDisplayToElement(arena, (dialogueBox_groupPtr + i)->displayData, ids[j + k++]);
        j += k;
    }

    // What next  to allocate?

    for (int i = 0; i < 10; i++)
    {
        if (!(layoutButtons_groupPtr + i)->btnData)
            continue;

        (layoutButtons_groupPtr + i)->btnData->currentState = BSTATE_NORMAL;
        (layoutButtons_groupPtr + i)->btnData->previousState = BSTATE_NONE;
    }

    // AssignTextData(txtDsply, newLayoutTxt,NULL, 20f, )
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
