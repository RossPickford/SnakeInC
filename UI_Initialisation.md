# UI Initilasation Process

This outlines how UI_Elements are initialised in this project

## Arena

All UI data for a layout is stored within an Arena. An Arena of appropriate size must be  
created to be used for all UI elements and their corresponding Data.

## UI Element Groups

UI Elements should be "grouped" - ideally this is a collection of elements that react together to the same  
input, or are visually grouped together.

A UI Element Group is not a struct or variable, but rather a pointer to a chunk of memory allocated in the arena to the size of  
all UI Elements within the group. For example, for a dialogue box that requires a box element, a text element, and an input element  
 would be the size of 3 UI_Elements.

## Button Data Allocation

After allocating the space for the UI Elements, memory must be allocated for the UI_Elements that are buttons. The button data pointer  
within the UI_Element struct must point to the allocated memory for the data. All button UI_Elements must have their button data allocated  
linearly after the initial UI_Element allocation.  

## Display Data Allocation

Then for each UI_Element, memory for the display data must be allocated. Again this should be done linearly as some elements will have  
multiple display datas.