#include "window.h"
#include <stdio.h> // Include for file handling

static const int GRID_WIDTH = 32;
static const int GRID_HEIGHT = 32;
static const int TILE_SIZE = 16; // Size of each tile in the tilemap

int **grid; // 2D array for storing the grid's state

void initialize_grid(int width, int height)
{
    grid = malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++)
    {
        grid[i] = malloc(height * sizeof(int));
    }
}

void free_grid(int width)
{
    for (int i = 0; i < width; i++)
    {
        free(grid[i]);
    }
    free(grid);
}

void read_grid_state(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        SDL_Log("Error opening file: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            fscanf(file, "%2x", &grid[j][i]);
        }
    }

    fclose(file);
}

void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level,
                 int offset_x, int offset_y) {
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    int tile_size = 32 * zoom_level;

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float) tile_size, (float) tile_size};

            // Select the correct texture based on the tile type
            int tile_type = grid[i][j];
            int src_x = (tile_type % tilemap_width) * TILE_SIZE;
            int src_y = (tile_type / tilemap_width) * TILE_SIZE;

            SDL_FRect src = {src_x, src_y, TILE_SIZE, TILE_SIZE};
            SDL_RenderTexture(renderer, tilemap, &src, &dest);
        }
    }
}

void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y) {
    *grid_x = character_x / tile_size;
    *grid_y = character_y / tile_size;
}

void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level,
                           int offset_x, int offset_y) {
    SDL_FRect highlight_rect = {
        offset_x + grid_x * tile_size * zoom_level,
        offset_y + grid_y * tile_size * zoom_level,
        tile_size * zoom_level,
        tile_size * zoom_level
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderRect(renderer, &highlight_rect);

    // Draw additional lines to make the border 3 pixels wide
    highlight_rect.x -= 1;
    highlight_rect.y -= 1;
    highlight_rect.w += 2;
    highlight_rect.h += 2;
    SDL_RenderRect(renderer, &highlight_rect);

    highlight_rect.x -= 1;
    highlight_rect.y -= 1;
    highlight_rect.w += 2;
    highlight_rect.h += 2;
    SDL_RenderRect(renderer, &highlight_rect);
}
