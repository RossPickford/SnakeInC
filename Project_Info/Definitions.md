# Definitions

This is a comprehensive list of all Structs and funcitons pertained to this project.

The document is sorted by header files in alphabetical order

## arena.h

### struct Arena

An Arena is an allocated chunk of memory used to linearly allocate data when required, and then free all allocated data at once

| Variables     | Descriptions  |
| ------------- | ------------- |
| void *start   | A pointer to the start of the arena  |
| size_t size   | The total size of the arena in bytes |
| size_t offset | The size of memory allocated in the area by the user in bytes |

### void createArena(Arena *arena, size_t size)

Allocates the memory in the heap for a given arena.

| Parameters     | Descriptions  |
| -------------- | ------------- |
| Arena *arena   | The arena to be allocated memory for |
| size_t size    | The size of the memory to be allocated in bytes |

### void arenaAlloc(Arena *arena, size_t size)

Returns a void pointer to the requested chunk of memory in the arena. This does not actually allocate any memory but rather pushes the offset by the size requested.

| Parameters     | Descriptions  |
| -------------- | ------------- |
| Arena *arena   | The arena to allocate memory in |
| size_t size    | The size of data to be allocated in bytes |

### void freeArena(Arena *arena)

Resets the offset back to 0, essentially "freeing" the user allocated memory.

| Parameters     | Descriptions  |
| -------------- | ------------- |
| Arena *arena   | The arena to free |

### void destroyArena(Arena *arena)

This will free the entire Arena and it will no longer be a valid arena to use

| Parameters     | Descriptions  |
| -------------- | ------------- |
| Arena *arena   | The arena to destroy |

## MainMenuLayout.h

### extern UI_Element *layoutButtons_groupPtr

This is the starting pointer for all UI_Elements in the main menu layout, however, as the name indicates, the first two congruent elements are the a part of the main layout.

### extern UI_Element *dialogueBox_groupPtr

This is the pointer to all UI_Elements in the dialogue box - displayed after selecting "new layout"

### bool initMainMenuWidgets(Arena *arena, SDL_Renderer *renderer, size_t width, size_t height)

This will initialise all UI_Elements configured for the main menu layout. You can then use the LayoutButtons_groupPtr and the dialogueBox_groupPtr pointers to access the elements to alter and render.

| Parameters     | Descriptions  |
| -------------- | ------------- |
| Arena *arena   | The arena used to allocate memory for UI_Elements and other relevant data |
| SDL_Renderer *renderer  | The renderer associated with the window this layout is presented in |
| size_t width   | The width of the associated window |
| size_t height  | the height of the associated window |

## ui_types.h

### struct UI_Element

This holds all relevant data for a single UI widget. 

| Variables             | Descriptions        |
| --------------------- | ------------------- |
| SDL_Texture *texture  | The texture to be rendered - compiled from all displayData |
| SDL_FRect rect        | The rect to use with the texture when rendering
| size_t displayCount   | The number of display data types in this element
| displayType *displayData | A pointer to the display type/s |
| ButtonData *btnData   | A pointer to the button data, NULL if element is not a button |

Both the displayData and buttonData need to be allocated seperately and then pointed to. This is done in the same arena the UI_Elements are allocated. See [UI Initialisation](UI_Initialisation) for more information

### struct ButtonData

The button data for a [UI_Element](#struct-ui_element).

| Variables             | Descriptions        |
| --------------------- | ------------------- |
| ButtonState currentState | The current state of the button |
| ButtonState previousState | The previous state of the button |
| SDL_Color **colours   | A pointer to an array of pointers to colours for each state of the button |

the size of colours should use displayCount from [UI_Element](#struct-ui_element).

### enum ButtonState

the possible states a button can be in

| Constant          | Description      |
| ----------------- | ---------------- |
| BSTATE_PRESSED    | The button is pressed |
| BSTATE_RELEASED   | The button is released |
| BSTATE_HOVERING   | The mouse is hovering over the button |
| BSTATE_NORMAL     | The button as displayed |
| BSTATE_NONE       | The button has no state |

### struct DisplayType

A generic struct that stores the display type and a pointer to the actual data

| Variables             | Descriptions        |
| --------------------- | ------------------- |
| displayType_ID id     | the display type id |
| void *displayData     | a pointer to the display data |

The displayData pointer will need to be cast to the correct display type.

### enum displayType_ID

The different display types

| Constant          | Description      |
| ----------------- | ---------------- |
| DI_TEXT           | [text](#struct-text) |
| DI_BOX            | [box](#struct-box) |
| DI_IMAGE          | [image](#struct-image) |

### struct Text

Text data to display

| Variable          | Description      |
| ----------------- | ---------------- |
| char *text        | The text to display |
| char *fontFile    | Link to the font to use |
| float fontSize    | The font size |
| SDL_Color *colour | The text colour |

### struct Box

Box data to display

| Constant          | Description      |
| ----------------- | ---------------- |
| float width       | The width of the box |
| float height      | The height of the box |
| SDL_Color *edgeColour | The colour of the box edges |
| SDL_Color *fillColor | The colour to fill the box |

### struct Image

N/A

### size_t getTypeFromID(displayType_ID id)

Returns the size of the display type based on the ID paramater given. The size is returned in bytes.

| Parameters        | Description      |
| ----------------- | ---------------- |
| displayType_ID    | The id of the display |

### void LinkAllocateDisplayToElement(Arena *arena, UI_Element *elmnt, displayType_ID *id, size_t displayCount)

This will allocate memory for the display types for a given UI_Element based on the display count provided. It will also then allocate the display data based on the id pointer - which must be an array - the size of displayCount.

| Parameters        | Description      |
| ----------------- | ---------------- |
| Arena *arena      | The arena to allocate memory |
| UI_Element *elmnt | The ui element to link and allocate the display data for |
| DisplayType_ID *id | An array of ids to link the display types to |
| size_t displayCount | The number of display data to allocate for |

### void AssignText(Text *txt, char *str, char *fontFile, float fontSize, SDL_Color *colour)


Assigns all variables in the given Text struct.

| Parameters        | Description      |
| ----------------- | ---------------- |
| Text *txt         | The Text struct to assign values to |
| char *str         | The string to point to |
| char *fontFile    | The font file to point to |
| float fontSize    | The font size to assign |
| SDL_Color *colour | The colour to point to |

### void AssignBox(Box *bx, float width, float height, SDL_Color *edgeColour, SDL_Color *fillColour)

Assign all variables in the given Box struct

| Parameters        | Description      |
| ----------------- | ---------------- |
| Box *bx           | The Box struct to assign values to |
| float width       | The width of the box |
| float height      | The height of the box |
| SDL_Color *edgeColour | The edge colour of the box |
| SDL_Color *fillColour | The fill colour of the box |

### float getTextHeight(Text *txt, SDL_Renderer *renderer)

Returns the height of the Text in the context of the renderer.

| Parameters        | Description      |
| ----------------- | ---------------- |
| Text *txt         | The Text to get the height from |
| SDL_Renderer *renderer | The rendering context the text is based in

### getTextWidth(Text *txt, SDL_Renderer *renderer)

Returns the width of the Text in the context of the renderer.

| Parameters        | Description      |
| ----------------- | ---------------- |
| Text *txt         | The Text to get the width from |
| SDL_Renderer *renderer | The rendering context the text is based in

### SDL_Surface *createTextSurface(Text *txt)

Returns a pointer to a SDL_Surface created from the inputed Text struct.

| Parameters        | Description      |
| ----------------- | ---------------- |
| Text *txt         | The Text to create a surface from |

### SDL_Surface *createBoxSurface(Box *bx)

Returns a pointer to a SDL_Surface created from the inputed Box struct

| Parameters        | Description      |
| ----------------- | ---------------- |
| Box *bx           | The Box to create a surface from |

### void createUITexture(UI_Element *ui, SDL_Renderer *renderer)

Creates and assigns a pointer to a texture to the UI_Element *texutre variable. The texture is created from all Display data assigned to the UI_Element.

| Parameters        | Description      |
| ----------------- | ---------------- |
| UI_Element *ui    | The UI element to create a texture from |
| SDL_Renderer *renderer | the rendering context to create the texture for |

### void setUITextureCoords(UI_Element *ui, float x, float y)

Assigns the inputed x and y coordinates to the UI_Element. This will be used when rendering the texture

| Parameters        | Description      |
| ----------------- | ---------------- |
| UI_Element *ui    | The UI_Element to assign the coordinates to |
| float x           | The X coordinate |
| float y           | The Y coordinate |