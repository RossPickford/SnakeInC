#include <SDL3/SDL_main.h>
#include "editorUI.h"

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

void InitAllText()
{
    InitButtonData(&createLayoutBtn, white, green, blue, NORMAL, NONE);
    InitButtonData(&loadLayoutBtn, white, green, blue, NORMAL, NONE);

    // InitTextData(&createLayoutBtn.textData, createLayoutTxt, )
}

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialise video: %s", SDL_GetError());
        return 1;
    }

    if (!TTF_Init())
    {
        SDL_Log("failed to load fonts: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("UI Editor", 1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_Color white = {255, 255, 255, 255};
    font = TTF_OpenFont("./fonts/PublicPixel.ttf", 20.0f);
    
    SDL_Surface *surf = TTF_RenderText_Solid(font, createLayoutTxt, 0, white);
    SDL_Texture *createLayoutTexture = SDL_CreateTextureFromSurface(renderer, surf);
    
    surf = TTF_RenderText_Solid(font, loadLayoutTxt, 0, white);
    SDL_Texture *loadLayoutTexture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_DestroySurface(surf);

    bool quit = false;
    while(!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        if ( event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            quit = true;
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_FRect rect;
        rect.x = 0.0f; 
        rect.y = 0.0f;

        SDL_GetTextureSize(createLayoutTexture, &rect.w, &rect.h);
        SDL_RenderTexture(renderer, createLayoutTexture, NULL, &rect);

        rect.y += (rect.h * 1.5f);

        SDL_GetTextureSize(loadLayoutTexture, &rect.w, &rect.h);
        SDL_RenderTexture(renderer, loadLayoutTexture, NULL, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    return 0;
}