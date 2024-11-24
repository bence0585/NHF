#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "window/window.h"
#include "debugmalloc.h"
/*
 * A program belépési pontja
 * @param argc argumentumok száma
 * @param argv argumentumok tömbje
 * @return 0, ha sikeres, egyébként negatív szám
 */
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
    if (background_grid == NULL)
    {
        SDL_Log("Failed to create background grid");
        return -6;
    }

    ForegroundGrid *foreground_grid = create_foreground_grid(grid_width, grid_height);
    if (foreground_grid == NULL)
    {
        SDL_Log("Failed to create foreground grid");
        destroy_grid(background_grid);
        return -7;
    }

    read_grid_state("../src/grid_state.txt", background_grid);     // háttér rács beolvasása
    read_collision_data("../src/collisions.txt", background_grid); // ütközési adatok beolvasása
    read_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid);

    SDL_Log("SDL3 init");

    event_loop(renderer, background_grid, foreground_grid);

    SDL_Log("SDL3 shutdown");
    cleanup_font();
    cleanup(renderer, window);

    SDL_Log("Destroying background grid");
    destroy_grid(background_grid);
    SDL_Log("Destroying foreground grid");
    destroy_foreground_grid(foreground_grid);

    SDL_Log("Exiting main function");
    return 0;
}