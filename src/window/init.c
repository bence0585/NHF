#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

int initialize_SDL()
{
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0)
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
    }
    return result;
}
int initialize_SDLTTF()
{
    int result = TTF_Init();
    if (result < 0)
    {
        SDL_Log("TTF_Init() Error: %s", SDL_GetError());
        return -1;
    }
    return 0;
}
SDL_Window *create_window(const char *title, int width, int height)
{
    SDL_Window *window = SDL_CreateWindow(title, width, height, 0);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
    }
    return window;
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
    }
    return renderer;
}

void cleanup(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}