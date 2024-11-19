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

    int grid_width, grid_height;
    determine_grid_size("../src/grid_state.txt", &grid_width, &grid_height);

    Grid *background_grid = create_grid(grid_width, grid_height);
    ForegroundGrid *foreground_grid = create_foreground_grid(grid_width, grid_height);

    read_grid_state("../src/grid_state.txt", background_grid);
    read_collision_data("../src/collisions.txt", background_grid); // Read collision data
    read_foreground_grid_state("../src/foregrid_state.txt", foreground_grid);

    SDL_Log("SDL3 init");

    event_loop(renderer, background_grid, foreground_grid);

    SDL_Log("SDL3 shutdown");
    cleanup_font();
    cleanup(renderer, window);

    destroy_grid(background_grid);
    destroy_foreground_grid(foreground_grid);

    return 0;
}