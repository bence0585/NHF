#include "window.h"
#include <stdio.h>
#include <stdlib.h>

Grid *create_grid(int width, int height)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->width = width;
    grid->height = height;

    grid->physical_layer = (int **)malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++)
    {
        grid->physical_layer[i] = (int *)malloc(height * sizeof(int));
    }

    return grid;
}

void destroy_grid(Grid *grid)
{
    for (int i = 0; i < grid->width; i++)
    {
        free(grid->physical_layer[i]);
    }
    free(grid->physical_layer);
    free(grid);
}

void determine_grid_size(const char *filename, int *width, int *height)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Grid_state file hiba: %s", strerror(errno));
        return;
    }

    *width = 0;
    *height = 0;
    char ch;
    int current_width = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            (*height)++;
            if (current_width > *width)
            {
                *width = current_width;
            }
            current_width = 0;
        }
        else if (ch != ' ')
        {
            current_width++;
        }
    }
    *width /= 2; // Each tile is represented by 2 hex digits
    fclose(file);
}

void read_grid_state(const char *filename, Grid *grid)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Grid_state file hiba: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            fscanf(file, "%2x", &grid->physical_layer[j][i]);
        }
    }

    fclose(file);
}

void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)
{
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    int tile_size = tilemap_width * zoom_level;

    for (int i = 0; i < grid->width; i++)
    {
        for (int j = 0; j < grid->height; j++)
        {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float)tile_size, (float)tile_size};

            int tile_type = grid->physical_layer[i][j];
            int src_x = (tile_type % tilemap_width) * tilemap_width;
            int src_y = (tile_type / tilemap_width) * tilemap_height;

            SDL_FRect src = {src_x, src_y, (float)tilemap_width, (float)tilemap_height};
            SDL_RenderTexture(renderer, tilemap, &src, &dest);
        }
    }
}

void render_visible_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y, int screen_width, int screen_height)
{
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    int tile_size = tilemap_width * zoom_level;

    int start_x = -offset_x / tile_size;
    int start_y = -offset_y / tile_size;
    int end_x = (screen_width - offset_x) / tile_size + 1;
    int end_y = (screen_height - offset_y) / tile_size + 1;

    if (start_x < 0)
        start_x = 0;
    if (start_y < 0)
        start_y = 0;
    if (end_x > grid->width)
        end_x = grid->width;
    if (end_y > grid->height)
        end_y = grid->height;

    for (int i = start_x; i < end_x; i++)
    {
        for (int j = start_y; j < end_y; j++)
        {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float)tile_size, (float)tile_size};

            int tile_type = grid->physical_layer[i][j];
            int src_x = (tile_type % tilemap_width) * tilemap_width;
            int src_y = (tile_type / tilemap_width) * tilemap_height;

            SDL_FRect src = {src_x, src_y, (float)tilemap_width, (float)tilemap_height};
            SDL_RenderTexture(renderer, tilemap, &src, &dest);
        }
    }
}

void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)
{
    *grid_x = character_x / tile_size;
    *grid_y = character_y / tile_size;
}

void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)
{
    SDL_FRect highlight_rect = {
        offset_x + grid_x * tile_size * zoom_level,
        offset_y + grid_y * tile_size * zoom_level,
        tile_size * zoom_level,
        tile_size * zoom_level};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int i = 0; i < 3; i++)
    {
        SDL_RenderRect(renderer, &highlight_rect);
        highlight_rect.x -= 1;
        highlight_rect.y -= 1;
        highlight_rect.w += 2;
        highlight_rect.h += 2;
    }
}

void highlight_look_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)
{
    SDL_FRect highlight_rect = {
        offset_x + grid_x * tile_size * zoom_level,
        offset_y + grid_y * tile_size * zoom_level,
        tile_size * zoom_level,
        tile_size * zoom_level};

    SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255); // Pink color

    for (int i = 0; i < 3; i++)
    {
        SDL_RenderRect(renderer, &highlight_rect);
        highlight_rect.x -= 1;
        highlight_rect.y -= 1;
        highlight_rect.w += 2;
        highlight_rect.h += 2;
    }
}

void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type)
{
    grid->physical_layer[grid_x][grid_y] = tile_type;
}
