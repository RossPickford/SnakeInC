#include <stdio.h>
#include <sdl2/SDL.h>

int main(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow("my window", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

    if (!win)
    {
        printf("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface* winSurface = SDL_GetWindowSurface(win);

    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 90, 120));

    SDL_UpdateWindowSurface(win);

    system("pause");

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
    
}