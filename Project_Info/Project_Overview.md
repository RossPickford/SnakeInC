# Objective

To create a very basic UI editor that can save a  layout created by the user into a binary file - this file can then  
be loaded into another project using the editor API which will then display the layout as designed.

<span style="color:cyan">*I am debating on whether to save the logic into the binary file or to leave it to the user. I could utilise both and   
save basic internal logic (this button shows these widgets), and then let the user implement more specific/ complex   
logic that is intertwined with the user's codebase.*</span>

## Editor Capabilities

The editor should be able to create the following widgets:
- Text button
- Box button
- Text Input Box
- Text Display
- Box Display
- Image Display
- Slider
- Dropdown

The user can then edit the following data for each widget they create:
- position (by either typing it in the widget settings or dragging the widget)
- Scale
- Rotation.

<span style="color:cyan">*I am thinking of adding in motion - most likely for display widgets only for the time being.*</span>


## TO DO (Whenever I come back to this)

<span style="color:red">**Next task is to make the "new Layout" and "load layout" buttons to change colour and to aslo function as intended (open their respective dialogue boxes)**</span>