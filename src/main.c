#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "window/window.h"

int main(int argc, char *argv[])
{
    if (initialize_SDL() < 0)
    {
        return -1;
    }
    if (initialize_SDLTTF() < 0)
    {
        return -2;
    }
    if (initialize_font("../src/fonts/arial.ttf", 32) < 0)
    {
        return -3;
    }

    SDL_Window *window = create_window("Ablak", 1600, 900);
    if (window == NULL)
    {
        return -4;
    }

    SDL_Renderer *renderer = create_renderer(window);
    if (renderer == NULL)
    {
        return -5;
    }

    SDL_Log("SDL3 init");

    event_loop(renderer);

    SDL_Log("SDL3 shutdown");
    cleanup_font();
    cleanup(renderer, window);

    return 0;
}