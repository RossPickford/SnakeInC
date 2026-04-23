#include <SDL3/SDL_main.h>
#include "editorUI.h"

#define APP_CONTINUE true
#define APP_END false

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

SDL_DialogFileCallback fileCallback;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *font = NULL;

SDL_Color black = {0, 0, 0, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color red = {255, 0, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color blue = {0, 0, 255, 255};

ButtonData createLayoutBtn;
char *createLayoutTxt = "Create Layout";

ButtonData loadLayoutBtn;
char *loadLayoutTxt = "Load Layout...";

InputTextBoxData createLayoutInput;

SDL_FPoint mousePos;

bool initSystems()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialise video: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Log("failed to load fonts: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool InitAllText()
{
    font = TTF_OpenFont("./fonts/PublicPixel.ttf", 20.0f);

    AssignButtonData(&createLayoutBtn, white, green, blue, NORMAL, NONE);
    AssignButtonData(&loadLayoutBtn, white, green, blue, NORMAL, NONE);

    AssignTextData(&createLayoutBtn.textData, createLayoutTxt, font, 20.0f, createLayoutBtn.displayColour);
    AssignTextData(&loadLayoutBtn.textData, loadLayoutTxt, font, 20.0f, loadLayoutBtn.displayColour);

    InitInputTextBox(&createLayoutInput, MAX_CHAR_INPUT, font, 20.0f, &white);
    createLayoutInput.textBox.x = WINDOW_WIDTH / 3.0f;
    createLayoutInput.textBox.y = WINDOW_HEIGHT / 3.0f;

    return UpdateText(&createLayoutBtn.textData, renderer) &&
           UpdateText(&loadLayoutBtn.textData, renderer) &&
           SetTextPosition(&createLayoutBtn.textData, 0.0f, 0.0f) &&
           SetTextPosition(&loadLayoutBtn.textData, 0.0f, (createLayoutBtn.textData.rect.h * 1.5f));
}

bool AppInit()
{
    if (!initSystems())
        return APP_END;

    if (!SDL_CreateWindowAndRenderer("UI Editor", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return APP_END;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!InitAllText())
        return APP_END;

    return APP_CONTINUE;
}

static const SDL_DialogFileFilter filters[] = {
    {"PNG images", "png"},
    {"JPEG images", "jpg;jpeg"},
    {"All images", "png;jpg;jpeg"},
    {"All files", "*"}};

static void SDLCALL callback(void *userdata, const char *const *filelist, int filter)
{
    if (!filelist)
    {
        SDL_Log("An error occured: %s", SDL_GetError());
        return;
    }
    else if (!*filelist)
    {
        SDL_Log("The user did not select any file.");
        SDL_Log("Most likely, the dialog was canceled.");
        return;
    }

    while (*filelist)
    {
        SDL_Log("Full path to selected file: '%s'", *filelist);
        filelist++;
    }

    if (filter < 0)
    {
        SDL_Log("The current platform does not support fetching "
                "the selected filter, or the user did not select"
                " any filter.");
    }
    else if (filter < SDL_arraysize(filters))
    {
        SDL_Log("The filter selected by the user is '%s' (%s).",
                filters[filter].pattern, filters[filter].name);
    }
}

bool AppEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            return APP_END;

        if (event.type == SDL_EVENT_KEY_DOWN)
            InsertCharacter(&createLayoutInput, SDL_GetKeyName(event.key.key));

        SDL_ConvertEventToRenderCoordinates(renderer, &event);
        mousePos.x = event.button.x;
        mousePos.y = event.button.y;

        if (CheckButtonState(&createLayoutBtn, &event, &mousePos))
            ;
        if (CheckButtonState(&loadLayoutBtn, &event, &mousePos) && loadLayoutBtn.currentState == RELEASED)
        {
            SDL_Log("clicked on");
            SDL_ShowOpenFileDialog(callback, NULL, window, NULL, 0, NULL, false);
        }
    }

    return APP_CONTINUE;
}

bool AppIterate()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_FRect rect;

    rect.x = rect.y = 0.0f;
    rect.w = WINDOW_WIDTH;
    rect.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    RenderInputTextBox(&createLayoutInput, renderer);

    if (createLayoutBtn.currentState != createLayoutBtn.previousState)
        UpdateText(&createLayoutBtn.textData, renderer);

    if (loadLayoutBtn.currentState != loadLayoutBtn.previousState)
        UpdateText(&loadLayoutBtn.textData, renderer);

    SDL_RenderTexture(renderer, createLayoutBtn.textData.texture, NULL, &createLayoutBtn.textData.rect);
    SDL_RenderTexture(renderer, loadLayoutBtn.textData.texture, NULL, &loadLayoutBtn.textData.rect);

    SDL_RenderPresent(renderer);

    return APP_CONTINUE;
}

int main(int argc, char *argv[])
{
    bool status = AppInit();

    // While status is true it will loop
    while (status)
        status = AppEvent() && AppIterate();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();

    return 0;
}