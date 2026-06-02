# UI Initilasation Process

This outlines how [UI_Elements](Definitions) are initialised in this project

## Arena

All UI data for a layout is stored within an Arena. An Arena of appropriate size must be created to be used for all UI elements and their corresponding Data using **createArena**

~~~
Arena arena;
createArena(&arena, 2000);
~~~

## UI Element Groups

UI elements should be "grouped" - ideally this is a collection of elements that react together to the same input, or are visually grouped together.

A UI element Group is not a struct or variable, but rather a **UI_Element** pointer to a linearly allocated memory in the arena the size of all elements within the group. For example, a dialogue box that requires a box element, a text element, and an input element would be the size of 3 **UI_Elements**.

~~~
#define ELEMENT_COUNT 3

...

UI_Element *elementGrpPtr = (UI_Element *)arenaAlloc(&arena, sizeof(UI_Element) * ELEMENT_COUNT);
~~~

## Display Data Allocation

Then for each UI_Element, memory for the display data must be allocated. Again this should be done linearly as some elements will have multiple display datas.

~~~
DisplayType_ID ids[] = {
    DI_BOX,             // Main display box
    DI_TEXT, DI_BOX     // Text Box button
    DI_TEXT             // Title Text
};

size_t idSize[] = {     // Number of Display Types for each
    1,                  // UI_Element - directly tied with 
    2,                  // the ids array
    1
};

for (size_t i = 0, j = 0; i < ELEMENT_COUNT; i++)
{
    LinkAllocateDisplayToElement(&arena, (elementGrpPtr + i), (ids + j) idSize[i]);
    j += idSize[i];
}
~~~

## Button Data Allocation

After allocating display data, memory must be allocated for the UI_Elements that are buttons. The button data pointer  
within the UI_Element struct must point to the allocated memory for the data. All button UI_Elements must have their button data allocated linearly after the initial UI_Element allocation.  

~~~
// This assigns the text box button as a button
(ElementGrpPtr + 1)->btnData = (ButtonData *)arenaAlloc(&arena, sizeof(ButtonData));
~~~

## Creating SDL_Textures For Each Element

Once all the necessary memory has been allocated, use **createUITexture** to create the SDL_Texture and to automatically assign them to the UI_Element's texture variable. This can then be used to render the elements.

~~~
for (size_t i = 0; i < ELEMENT_COUNT; i++)
    createUITexture((elementGrpPtr + i), renderer);
~~~