#include "window.h"
#include <stdio.h>
#include <stdlib.h>

Grid *create_grid(int width, int height)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->width = width;
    grid->height = height;

    grid->physical_layer = (int **)malloc(width * sizeof(int *));
    grid->collision_layer = (bool **)malloc(width * sizeof(bool *)); // Allocate memory for collision layer
    for (int i = 0; i < width; i++)
    {
        grid->physical_layer[i] = (int *)malloc(height * sizeof(int));
        grid->collision_layer[i] = (bool *)malloc(height * sizeof(bool)); // Allocate memory for collision layer
        for (int j = 0; j < height; j++)
        {
            grid->collision_layer[i][j] = false; // Initialize all cells as passable
        }
    }

    return grid;
}

void destroy_grid(Grid *grid)
{
    for (int i = 0; i < grid->width; i++)
    {
        free(grid->physical_layer[i]);
        free(grid->collision_layer[i]); // Free memory for collision layer
    }
    free(grid->physical_layer);
    free(grid->collision_layer); // Free memory for collision layer
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
    *grid_x = (character_x / tile_size);
    *grid_y = (character_y / tile_size);
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
    SDL_Log("Set tile type at (%d, %d) to %d", grid_x, grid_y, tile_type); // Log the tile type change
}

TileType get_tile_type(Grid *grid, int grid_x, int grid_y)
{
    return grid->physical_layer[grid_x][grid_y];
}

void update_tile_texture(Grid *grid, int grid_x, int grid_y)
{
    // Update the texture based on the neighbors
    TileType current_tile = get_tile_type(grid, grid_x, grid_y);

    // Example logic to update the texture based on neighbors
    // This can be expanded based on the specific requirements for texture updates
    if (current_tile == TILE_HOE)
    {
        // Check neighbors and update texture accordingly
        // For simplicity, let's assume we just log the update for now
        SDL_Log("Updating texture for hoed tile at (%d, %d)", grid_x, grid_y);
    }
}

void on_tile_change(Grid *grid, int grid_x, int grid_y, TileType tile_type)
{
    // Handle changes in the grid system
    // For example, update neighbors or trigger other events
    // ...implement the logic here...
}

ForegroundGrid *create_foreground_grid(int width, int height)
{
    ForegroundGrid *grid = (ForegroundGrid *)malloc(sizeof(ForegroundGrid));
    grid->width = width;
    grid->height = height;

    grid->background_layer = (int **)malloc(width * sizeof(int *));
    grid->foreground_layer = (int **)malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++)
    {
        grid->background_layer[i] = (int *)malloc(height * sizeof(int));
        grid->foreground_layer[i] = (int *)malloc(height * sizeof(int));
    }

    return grid;
}

void destroy_foreground_grid(ForegroundGrid *grid)
{
    for (int i = 0; i < grid->width; i++)
    {
        free(grid->background_layer[i]);
        free(grid->foreground_layer[i]);
    }
    free(grid->background_layer);
    free(grid->foreground_layer);
    free(grid);
}

void render_foreground_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, ForegroundGrid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)
{
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    int tile_size = tilemap_width * zoom_level;

    for (int i = 0; i < grid->width; i++)
    {
        for (int j = 0; j < grid->height; j++)
        {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float)tile_size, (float)tile_size};

            int tile_type = grid->foreground_layer[i][j];
            int src_x = (tile_type % tilemap_width) * tilemap_width;
            int src_y = (tile_type / tilemap_width) * tilemap_height;

            SDL_FRect src = {src_x, src_y, (float)tilemap_width, (float)tilemap_height};
            SDL_RenderTexture(renderer, tilemap, &src, &dest);
        }
    }
}

bool check_collision(ForegroundGrid *grid, int grid_x, int grid_y)
{
    if (grid_x < 0 || grid_x >= grid->width || grid_y < 0 || grid_y >= grid->height)
    {
        return true; // Out of bounds is considered a collision
    }
    return grid->foreground_layer[grid_x][grid_y] != 0; // Check collision based on tile type
}

void read_foreground_grid_state(const char *filename, ForegroundGrid *grid)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Foreground grid state file error: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            fscanf(file, "%2x", &grid->foreground_layer[j][i]);
        }
    }

    fclose(file);
}

void read_collision_data(const char *filename, Grid *grid)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Collision data file error: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            int collision;
            fscanf(file, "%2x", &collision);                   // Read a 2-digit hex number
            grid->collision_layer[j][i] = (collision == 0xFF); // Set collision based on tile type
        }
    }

    fclose(file);
}

void toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y)
{
    grid->collision_layer[grid_x][grid_y] = !grid->collision_layer[grid_x][grid_y];

    SDL_Log("Toggling collision at (%d, %d)", grid_x, grid_y); // Log the coordinates

    FILE *file = fopen(filename, "r+");
    if (file == NULL)
    {
        SDL_Log("Collision data file error: %s", strerror(errno));
        return;
    }

    int line_length = grid->width * 3;                              // Each 2-digit hex number is followed by a space
    fseek(file, grid_y * (line_length + 1) + grid_x * 3, SEEK_SET); // Adjust for newline characters
    fprintf(file, "%02X", grid->collision_layer[grid_x][grid_y] ? 0xFF : 0x00);

    fclose(file);
}

void save_grid_state(const char *filename, Grid *grid)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Failed to open grid state file for writing: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            fprintf(file, "%02x ", grid->physical_layer[j][i]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    SDL_Log("Saved grid state to %s", filename); // Log the save action
}

void save_foreground_grid_state(const char *filename, ForegroundGrid *grid)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Failed to open foreground grid state file for writing: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            fprintf(file, "%02x ", grid->foreground_layer[j][i]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
