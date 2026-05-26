#include "ui_editor.h"
#include "mainMenuLayout.h"

#define APP_CONTINUE true
#define APP_END false

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

SDL_DialogFileCallback fileCallback;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_FPoint *mousePosPtr = NULL;
float *mousePos_x = NULL;
float *mousePos_y = NULL;

static Arena widgetArena;

bool processRenderBuffer(UI_Element *rendBuff, size_t offset)
{
    for (size_t i = 0; i < offset; i++)
    {
        if (!SDL_RenderTexture(renderer, (rendBuff + i)->texture, NULL, &(rendBuff + i)->rect))
            return false;
    }

    return true;
}

bool queryAction(UIType_ID *id)
{
    switch (id->action)
    {
    case IACTION_SHOW_CHILDREN:
        if (id->children == NULL)
            break;
        // showChildren(id->children);
    }
}

void processInputEvents(UIType_ID *id)
{
    switch (id->type)
    {
    case ITYPE_BUTTON_TEXT:
        TextButton *btnData = (TextButton *)id->data;
        if (CheckButtonState(btnData, NULL, NULL))
        {
            UpdateText(&btnData->textData, renderer);
            if (btnData->currentState == BSTATE_RELEASED && queryAction(id))
                ;
        }
        break;
    }
}

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

bool AppInit()
{

    if (!initSystems())
        return APP_END;

    if (!SDL_CreateWindowAndRenderer("UI Editor", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return APP_END;
    }

    createArena(&widgetArena, 5000);

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!InitMainMenuWidgets(&widgetArena, renderer, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        SDL_Log("Failed to initialse main menu");
        return APP_END;
    }

    return APP_CONTINUE;
}

static const SDL_DialogFileFilter filters[] = {
    {"PNG images", "png"},
    {"JPEG images", "jpg;jpeg"},
    {"All images", "png;jpg;jpeg"}};

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

bool showWindow = false;

bool AppEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            return APP_END;
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

    if (!processRenderBuffer(getMainMenuElements(), MAINMENU_ELEMENT_COUNT))
    {
        SDL_Log("failed to render texture: %s", SDL_GetError());
        return APP_END;
    }

    SDL_RenderPresent(renderer);

    return APP_CONTINUE;
}

int main(int argc, char *argv[])
{
    bool status = AppInit();

    // While status is true it will loop
    while (status)
        status = AppEvent() && AppIterate();

    destroyArena(&widgetArena);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();

    return 0;
}