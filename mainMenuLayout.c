#include "mainMenuLayout.h"

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

void InitMainMenuWidgets(Arena *arena)
{
    TextDisplay *txtDsply = (TextDisplay *)ArenaAlloc(arena, sizeof(TextDisplay) * 2);
    
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

