#include "window.h"
#include <stdio.h> // Include for file handling

#define GRID_WIDTH 32
#define GRID_HEIGHT 32

int grid[GRID_WIDTH][GRID_HEIGHT]; // 2D array for storing the grid's state

void read_grid_state(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Error opening file: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            fscanf(file, "%d", &grid[j][i]);
        }
    }

    fclose(file);
}

void render_grid(SDL_Renderer *renderer, SDL_Texture **textures, double zoom_level, int offset_x, int offset_y)
{
    SDL_Texture *tilemap = load_texture(renderer, "src/img/tilemap.png");
    int tile_size = 64 * zoom_level;
    SDL_FRect test[] = {16, 16, 16, 16};
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float)tile_size, (float)tile_size};

            // Select the correct texture based on the tile type
            int tile_type = grid[i][j];
            if (tile_type >= 0 && tile_type < 4) // Ensure the tile type is within the textures array bounds
            {
                SDL_RenderTexture(renderer, tilemap, test, &dest);
            }
        }
    }
}