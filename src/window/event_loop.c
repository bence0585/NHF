#include <stdio.h>
#include "window.h"

/*
 * A fő eseménykezelő függvény
 * @param renderer: a kirajzolást végző renderer
 */
void event_loop(SDL_Renderer *renderer)
{
    SDL_Event event;
    int quit = 0;
    double zoom_level = 1;
    int character_x = 640, character_y = 360; // Ez is változni fog
    int tile_size = 32;

    SDL_Texture *character_texture = load_texture(renderer, "../src/img/character.png");

    SDL_Texture *tilemap = load_texture(renderer, "../src/img/tilemap.png");
    const int tilemap_width = 16;
    const int tilemap_height = 16;

    read_grid_state("../src/grid_state.txt");

    /*
    * A fő eseménykezelő ciklus

    */
    while (!quit)
    {
        SDL_PumpEvents();
        const bool *state = SDL_GetKeyboardState(NULL);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                SDL_Log("Kilepes: SDL3 event");
                quit = 1;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    SDL_Log("Kilepes: SDL3 ESC gomb");
                    quit = 1;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (is_button_clicked(BUTTON_ZOOM_IN, x, y))
                    {
                        zoom_level += 1;
                    }
                    else if (is_button_clicked(BUTTON_ZOOM_OUT, x, y))
                    {
                        zoom_level -= 1;
                    }
                    // Add more button checks here
                }
            }
        }

        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
        {
            character_y -= 2;
        }
        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
        {
            character_y += 2;
        }
        if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
        {
            character_x -= 2;
        }
        if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
        {
            character_x += 2;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0x0, 0x0);
        SDL_RenderClear(renderer);

        int screen_width, screen_height;
        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character_x * zoom_level - (tile_size * zoom_level) / 2;
        int offset_y = screen_height / 2 - character_y * zoom_level - (tile_size * zoom_level) / 2;

        render_grid(renderer, tilemap, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);

        int grid_x, grid_y;
        convert_to_grid_coordinates(character_x, character_y, tile_size, &grid_x, &grid_y);

        highlight_grid_square(renderer, grid_x, grid_y, tile_size, zoom_level, offset_x, offset_y);

        SDL_FRect character_rect = {
            (float)(screen_width / 2 - (tile_size * zoom_level) / 2),
            (float)(screen_height / 2 - (tile_size * zoom_level) / 2),
            (float)(tile_size * zoom_level),
            (float)(tile_size * 2 * zoom_level)};
        SDL_RenderTexture(renderer, character_texture, NULL, &character_rect);

        render_button(renderer, BUTTON_ZOOM_IN);
        render_button(renderer, BUTTON_ZOOM_OUT);

        render_ui(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(tilemap);
}