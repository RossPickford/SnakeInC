#include <SDL3/SDL_main.h>
#include "editorUI.h"

#define APP_CONTINUE true
#define APP_END false

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

    return UpdateText(&createLayoutBtn.textData, renderer) &&
           UpdateText(&loadLayoutBtn.textData, renderer);
}

bool AppInit()
{
    if (!initSystems())
        return APP_END;

    if (!SDL_CreateWindowAndRenderer("UI Editor", 1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return APP_END;
    }

    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!InitAllText())
        return APP_END;

    return APP_CONTINUE;
}

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_FRect rect;
    rect.x = 0.0f;
    rect.y = 0.0f;

    SDL_GetTextureSize(createLayoutBtn.textData.texture, &rect.w, &rect.h);
    SDL_RenderTexture(renderer, createLayoutBtn.textData.texture, NULL, &rect);

    rect.y += (rect.h * 1.5f);

    SDL_GetTextureSize(loadLayoutBtn.textData.texture, &rect.w, &rect.h);
    SDL_RenderTexture(renderer, loadLayoutBtn.textData.texture, NULL, &rect);

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