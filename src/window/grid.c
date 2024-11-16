#include "window.h"
#include <stdio.h>

int grid[32][32]; // BORZALMAS MEGOLDÁS, DE MOST EZ VAN ;(

/*
 * A rácsot feltölti a megadott számokkal
 * @param value: a rácsba írandó szám
 */
void read_grid_state(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Grid_state file hiba: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            fscanf(file, "%2x", &grid[j][i]);
        }
    }

    fclose(file);
}
/*
 * Kirajzolja a rácsot a megadott tilemap segítségével
 * @param renderer: a kirajzolást végző renderer
 * @param tilemap: a tilemap textúra
 * @param tilemap_width: a tilemap szélessége
 * @param tilemap_height: a tilemap magassága
 * @param zoom_level: a nagyítás mértéke
 * @param offset_x: az x tengelyen eltolás
 * @param offset_y: az y tengelyen eltolás
 */
void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y)
{
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    int tile_size = tilemap_width * zoom_level;

    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            SDL_FRect dest = {offset_x + i * tile_size, offset_y + j * tile_size, (float)tile_size, (float)tile_size};

            int tile_type = grid[i][j];
            int src_x = (tile_type % tilemap_width) * TILE_SIZE;
            int src_y = (tile_type / tilemap_width) * TILE_SIZE;

            SDL_FRect src = {src_x, src_y, TILE_SIZE, TILE_SIZE};
            SDL_RenderTexture(renderer, tilemap, &src, &dest);
        }
    }
}
/*
 * A megadott karakter koordinátákat átkonvertálja rács koordinátákra
 * @param character_x: a karakter x koordinátája
 * @param character_y: a karakter y koordinátája
 * @param tile_size: a rács mérete
 * @param grid_x: a rács x koordinátája
 * @param grid_y: a rács y koordinátája
 */
void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)
{
    *grid_x = character_x / tile_size;
    *grid_y = character_y / tile_size;
}

/*
 * Körberajzolja a négyzetet, amely a megadott koordinátákon található
 * A négyzetet piros színnel rajzolja ki
 * A négyzetet háromszor rajzolja ki, hogy vastagabb legyen
 * @param renderer: a kirajzolást végző renderer
 * @param grid_x: a négyzet x koordinátája a rácsban
 * @param grid_y: a négyzet y koordinátája a rácsban
 * @param tile_size: a négyzet mérete
 * @param zoom_level: a nagyítás mértéke
 * @param offset_x: az x tengelyen eltolás
 * @param offset_y: az y tengelyen eltolás
 */
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

/*
 * Körberajzolja a négyzetet, amely a megadott koordinátákon található
 * A négyzetet rózsaszín színnel rajzolja ki
 * A négyzetet háromszor rajzolja ki, hogy vastagabb legyen
 * @param renderer: a kirajzolást végző renderer
 * @param grid_x: a négyzet x koordinátája a rácsban
 * @param grid_y: a négyzet y koordinátája a rácsban
 * @param tile_size: a négyzet mérete
 * @param zoom_level: a nagyítás mértéke
 * @param offset_x: az x tengelyen eltolás
 * @param offset_y: az y tengelyen eltolás
 */
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
