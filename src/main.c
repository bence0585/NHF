#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "window/window.h"

int main(int argc, char *argv[])
{
    if (initialize_SDL() < 0)
    {
        return -1;
    }

    SDL_Window *window = create_window("Ablak", 1600, 900);
    if (window == NULL)
    {
        return -2;
    }

    SDL_Renderer *renderer = create_renderer(window);
    if (renderer == NULL)
    {
        return -3;
    }

    SDL_Log("SDL3 init");
    SDL_Log("SDL3ssss");

    event_loop(renderer);

    SDL_Log("SDL3 shutdown");
    cleanup(renderer, window);

    return 0;
}