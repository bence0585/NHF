#include "window.h"

void event_loop(SDL_Renderer *renderer)
{
    SDL_Event event;
    int quit = 0;
    double zoom_level = 1;
    int character_x = 640, character_y = 360; // Character starting position
    int tile_size = 32;

    // Load character texture
    SDL_Texture *character_texture = load_texture(renderer, "src/img/character.png");

    // Load tilemap texture
    SDL_Texture *tilemap = load_texture(renderer, "src/img/tilemap.png");
    int tilemap_width = 2;  // Number of tiles horizontally in the tilemap
    int tilemap_height = 2; // Number of tiles vertically in the tilemap

    // Read the grid state from a file
    read_grid_state("src/grid_state.txt");

    while (!quit)
    {
        SDL_PumpEvents(); // Update the state array
        const bool *state = SDL_GetKeyboardState(NULL);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                SDL_Log("SDL3 event quit");
                quit = 1;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    SDL_Log("SDL3 ESC key quit");
                    quit = 1;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (is_zoom_in_button_clicked(x, y))
                    {
                        zoom_level += 1;
                    }
                    else if (is_zoom_out_button_clicked(x, y))
                    {
                        zoom_level -= 1;
                    }
                }
            }
        }

        // Character movement
        if (state[SDL_SCANCODE_W])
        {
            character_y -= 2;
        }
        if (state[SDL_SCANCODE_S])
        {
            character_y += 2;
        }
        if (state[SDL_SCANCODE_A])
        {
            character_x -= 2;
        }
        if (state[SDL_SCANCODE_D])
        {
            character_x += 2;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0x0, 0x0);
        SDL_RenderClear(renderer);

        // Calculate the offset to center the grid on the character
        int screen_width, screen_height;
        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character_x * zoom_level - (tile_size * zoom_level) / 2;
        int offset_y = screen_height / 2 - character_y * zoom_level - (tile_size * zoom_level) / 2;

        // Render the grid using the tilemap
        render_grid(renderer, tilemap, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);

        // Convert character coordinates to grid coordinates
        int grid_x, grid_y;
        convert_to_grid_coordinates(character_x, character_y, tile_size, &grid_x, &grid_y);

        // Highlight the grid square the character is standing on
        highlight_grid_square(renderer, grid_x, grid_y, tile_size, zoom_level, offset_x, offset_y);

        // Render the character from the middle of its coordinates
        SDL_FRect character_rect = {
            (float)(screen_width / 2 - (tile_size * zoom_level) / 2),
            (float)(screen_height / 2 - (tile_size * zoom_level) / 2),
            (float)(tile_size * zoom_level),
            (float)(tile_size * 2 * zoom_level)};
        SDL_RenderTexture(renderer, character_texture, NULL, &character_rect);

        // Render UI elements
        render_ui(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Clean up textures
    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(tilemap);
}