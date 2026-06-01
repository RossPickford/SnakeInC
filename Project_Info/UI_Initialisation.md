# UI Initilasation Process

This outlines how **UI_Elements** are initialised in this project

## Arena

All UI data for a layout is stored within an Arena. An Arena of appropriate size must be created to be used for all UI elements and their corresponding Data using **createArena**

## UI Element Groups

UI elements should be "grouped" - ideally this is a collection of elements that react together to the same input, or are visually grouped together.

A UI element Group is not a struct or variable, but rather a **UI_Element** pointer to a linearly allocated memory in the arena the size of all elements within the group. For example, a dialogue box that requires a box element, a text element, and an input element would be the size of 3 **UI_Elements**.

## Display Data Allocation

Then for each UI_Element, memory for the display data must be allocated. Again this should be done linearly as some elements will have multiple display datas.

## Button Data Allocation

After allocating display data, memory must be allocated for the UI_Elements that are buttons. The button data pointer  
within the UI_Element struct must point to the allocated memory for the data. All button UI_Elements must have their button data allocated linearly after the initial UI_Element allocation.  