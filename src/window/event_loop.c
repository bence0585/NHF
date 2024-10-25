#include "window.h"

void event_loop(SDL_Renderer *renderer)
{
    SDL_Event event;
    int quit = 0;
    int zoom_level = 1;
    int character_x = 640, character_y = 360; // Character starting position
    int tile_size = 32;

    // Load multiple textures for different tile types
    SDL_Texture *textures[4];
    textures[0] = load_texture(renderer, "src/img/grass.bmp");
    textures[1] = load_texture(renderer, "src/img/water.bmp");
    textures[2] = load_texture(renderer, "src/img/tilled_soil.bmp");
    textures[3] = load_texture(renderer, "src/img/stone_path.bmp");

    // Load character texture
    SDL_Texture *character_texture = load_texture(renderer, "src/img/character.png");

    // Read the grid state from a file
    read_grid_state("src/grid_state.txt");

    while (!quit)
    {
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
                // Character movement
                else if (event.key.key == SDLK_UP)
                {
                    character_y -= 2;
                }
                else if (event.key.key == SDLK_DOWN)
                {
                    character_y += 2;
                }
                else if (event.key.key == SDLK_LEFT)
                {
                    character_x -= 2;
                }
                else if (event.key.key == SDLK_RIGHT)
                {
                    character_x += 2;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                // Handle zoom in/out buttons
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (is_zoom_in_button_clicked(event.button.x, event.button.y))
                    {
                        zoom_level++;
                    }
                    else if (is_zoom_out_button_clicked(event.button.x, event.button.y))
                    {
                        zoom_level--;
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0x0, 0x0);
        SDL_RenderClear(renderer);

        // Calculate the offset to center the grid on the character
        int screen_width, screen_height;
        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character_x * zoom_level - (tile_size * zoom_level) / 2;
        int offset_y = screen_height / 2 - character_y * zoom_level - (tile_size * zoom_level) / 2;

        // Render the grid using the textures
        render_grid(renderer, textures, zoom_level, offset_x, offset_y);

        // Render the character
        SDL_FRect character_rect = {(float)(screen_width / 2 - (tile_size * zoom_level) / 2), (float)(screen_height / 2 - (tile_size * zoom_level) / 2), (float)(tile_size * zoom_level), (float)(tile_size * zoom_level)};
        SDL_RenderTexture(renderer, character_texture, NULL, &character_rect);

        // Render UI elements
        render_ui(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    // Clean up textures
    for (int i = 0; i < 4; i++)
    {
        if (textures[i] != NULL)
        {
            SDL_DestroyTexture(textures[i]);
        }
    }
    SDL_DestroyTexture(character_texture);
}