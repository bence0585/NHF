#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include "../debugmalloc.h"

/*
 * Létrehoz egy rácsot a megadott szélességgel és magassággal.
 * Az összes cellát inicializálja üresként.
 * @param width a rács szélessége
 * @param height a rács magassága
 * @return a rács mutatója
 */
Grid *create_grid(int width, int height)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    if (grid == NULL)
    {
        SDL_Log("Failed to allocate memory for grid.");
        return NULL;
    }
    grid->width = width;
    grid->height = height;

    grid->physical_layer = (int **)malloc(width * sizeof(int *));
    grid->optical_layer = (int **)malloc(width * sizeof(int *));     // Allocate memory for optical layer
    grid->collision_layer = (bool **)malloc(width * sizeof(bool *)); // Allocate memory for collision layer
    if (grid->physical_layer == NULL || grid->optical_layer == NULL || grid->collision_layer == NULL)
    {
        SDL_Log("Failed to allocate memory for grid layers.");
        free(grid);
        return NULL;
    }

    for (int i = 0; i < width; i++)
    {
        grid->physical_layer[i] = (int *)malloc(height * sizeof(int));
        grid->optical_layer[i] = (int *)malloc(height * sizeof(int));     // Allocate memory for optical layer
        grid->collision_layer[i] = (bool *)malloc(height * sizeof(bool)); // Allocate memory for collision layer
        if (grid->physical_layer[i] == NULL || grid->optical_layer[i] == NULL || grid->collision_layer[i] == NULL)
        {
            SDL_Log("Failed to allocate memory for grid layer row %d.", i);
            destroy_grid(grid);
            return NULL;
        }
        for (int j = 0; j < height; j++)
        {
            grid->collision_layer[i][j] = false; // Initialize all cells as passable
        }
    }

    return grid;
}
/*
 * Felszabadítja a rácsot és annak összes celláját.
 * @param grid a rács mutatója
 * @return void
 * @note A rács felszabadítása után a rács mutatója NULL-ra lesz állítva.
 * @note Az optikai és ütközési rétegek memóriáját is felszabadítja.
 */
void destroy_grid(Grid *grid)
{
    if (grid == NULL)
    {
        SDL_Log("Grid is already NULL");
        return;
    }
    SDL_Log("Destroying grid with width: %d, height: %d", grid->width, grid->height);
    for (int i = 0; i < grid->width; i++)
    {
        free(grid->physical_layer[i]);
        free(grid->optical_layer[i]);   // Free memory for optical layer
        free(grid->collision_layer[i]); // Free memory for collision layer
    }
    free(grid->physical_layer);
    free(grid->optical_layer);   // Free memory for optical layer
    free(grid->collision_layer); // Free memory for collision layer
    free(grid);
    SDL_Log("Grid destroyed");
}

/*
    * Meghatározza a rács szélességét és magasságát a megadott fájl alapján.
    * @param filename a fájl neve
    * @param width a rács szélességének mutatója
    * @param height a rács magasságának mutatója
    * @return void
    * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.

*/
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
/*
 * Beolvassa a rács állapotát a megadott fájlból.
 * @param filename a fájl neve
 * @param grid a rács mutatója
 * @return void
 * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.
 */
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
    SDL_Log("Re-read grid state from %s", filename); // Log the re-read action
}
/*
 * Rács kirajzolása a megadott rajzolóra a megadott csempetérképpel.
 * @param renderer a rajzoló
 * @param tilemap a csempetérkép textúrája
 * @param grid a rács mutatója
 * @param tilemap_width a csempetérkép szélessége
 * @param tilemap_height a csempetérkép magassága
 * @param zoom_level nagyítási szint
 * @param offset_x x eltolás
 * @param offset_y y eltolás
 * @return void
 * @note A rács cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note A rács celláinak mérete megegyezik a csempetérkép méretével.
 * @note A rács cellái között nincs rés.
 */
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

/*
 * A látható rács kirajzolása a megadott rajzolóra a megadott csempetérképpel.
 * @param renderer a rajzoló
 * @param tilemap a csempetérkép textúrája
 * @param grid a rács mutatója
 * @param tilemap_width a csempetérkép szélessége
 * @param tilemap_height a csempetérkép magassága
 * @param zoom_level nagyítási szint
 * @param offset_x x eltolás
 * @param offset_y y eltolás
 * @param screen_width a képernyő szélessége
 * @param screen_height a képernyő magassága
 * @return void
 * @note A rács cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note A rács celláinak mérete megegyezik a csempetérkép méretével.
 * @note A rács cellái között nincs rés.
 * @note Csak a látható cellákat rajzolja ki.
 */
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
/*
 * A rács celláinak koordinátáit átalakítja rács koordinátákra.
 * @param character_x a karakter x koordinátája
 * @param character_y a karakter y koordinátája
 * @param tile_size a csempe mérete
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @return void
 */
void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y)
{
    *grid_x = (character_x / tile_size);
    *grid_y = (character_y / tile_size);
}
/*
    * A rács cella amin a játékos állt, kirajzolása.
    * @param renderer a rajzoló
    * @param grid_x a rács x koordinátája
    * @param grid_y a rács y koordinátája
    * @param tile_size a csempe mérete
    * @param zoom_level nagyítási szint
    * @param offset_x x eltolás
    * @param offset_y y eltolás
    * @return void
    * @note A kiemelés színe piros.
    * @note A kiemelés 3 pixel széles.

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
 * kijelöli a rács cellát, ahol a játékos néz.
 * @param renderer a rajzoló
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param tile_size a csempe mérete
 * @param zoom_level nagyítási szint
 * @param offset_x x eltolás
 * @param offset_y y eltolás
 * @return void
 * @note A kiemelés színe rózsaszín.
 * @note A kiemelés 3 pixel széles.
 * @note A kiemelés a nézési irányban 1 csempével van eltolva.
 */
void highlight_look_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y)
{
    SDL_FRect highlight_rect = {
        offset_x + grid_x * tile_size * zoom_level,
        offset_y + grid_y * tile_size * zoom_level,
        tile_size * zoom_level,
        tile_size * zoom_level};

    SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255); // Rózsaszín szín a nézett csempéhez

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
 * A rács celláinak típusát beállítja a megadott értékre.
 * @param grid a rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param tile_type a cella típusa
 * @return void
 * @note A cella típus változását naplózza.
 */
void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type)
{
    grid->physical_layer[grid_x][grid_y] = tile_type;
    SDL_Log("Set tile type at (%d, %d) to %d", grid_x, grid_y, tile_type); // Logolás
}

/*
 * A rács celláinak típusát lekéri.
 * @param grid a rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @return a cella típusa
 */
TileType get_tile_type(Grid *grid, int grid_x, int grid_y)
{
    return grid->physical_layer[grid_x][grid_y];
}

/*
 * A rács celláinak optikai és fizikai rétegét beállítja a megadott értékre.
 * @param grid a rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param optical_layer a cella optikai rétege
 * @return void
 * @note Az optikai réteg változását naplózza.
 */
void update_tile_texture(Grid *grid, int grid_x, int grid_y)
{
    TileType current_tile = get_tile_type(grid, grid_x, grid_y);

    if (current_tile == TILE_HOE)
    {
        grid->physical_layer[grid_x][grid_y] = 0xE5; // Kapált texture 0xEC
        grid->optical_layer[grid_x][grid_y] = 0x00;  //  Optikai réteg törlése (ha volt eddig valami ott) 0x00
        SDL_Log("Set background texture to 0xE5 and optical layer to 0x00 for hoed tile at (%d, %d)", grid_x, grid_y);
    }
}
/*
 * A háttér fölé kirajzolt réteg létrehozása
 * @param width a réteg szélessége
 * @param height a réteg magassága
 * @return a réteg mutatója
 * @note A réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note A réteg celláinak mérete megegyezik a csempetérkép méretével.
 */
ForegroundGrid *create_foreground_grid(int width, int height)
{
    ForegroundGrid *grid = (ForegroundGrid *)malloc(sizeof(ForegroundGrid));
    if (grid == NULL)
    {
        SDL_Log("Failed to allocate memory for foreground grid.");
        return NULL;
    }
    grid->width = width;
    grid->height = height;

    grid->background_layer = (int **)malloc(width * sizeof(int *));
    grid->foreground_layer = (int **)malloc(width * sizeof(int *));
    if (grid->background_layer == NULL || grid->foreground_layer == NULL)
    {
        SDL_Log("Failed to allocate memory for foreground grid layers.");
        free(grid);
        return NULL;
    }

    for (int i = 0; i < width; i++)
    {
        grid->background_layer[i] = (int *)malloc(height * sizeof(int));
        grid->foreground_layer[i] = (int *)malloc(height * sizeof(int));
        if (grid->background_layer[i] == NULL || grid->foreground_layer[i] == NULL)
        {
            SDL_Log("Failed to allocate memory for foreground grid layer row %d.", i);
            destroy_foreground_grid(grid);
            return NULL;
        }
    }

    SDL_Log("Created foreground grid with width: %d, height: %d", width, height);
    return grid;
}
/*
 * A háttér fölé kirajzolt réteg felszabadítása
 * @param grid a réteg mutatója
 * @return void
 * @note A réteg felszabadítása után a réteg mutatója NULL-ra lesz állítva.
 */
void destroy_foreground_grid(ForegroundGrid *grid)
{
    if (grid == NULL)
    {
        SDL_Log("Foreground grid is already NULL");
        return;
    }
    SDL_Log("Destroying foreground grid with width: %d, height: %d", grid->width, grid->height);
    for (int i = 0; i < grid->width; i++)
    {
        free(grid->background_layer[i]);
        free(grid->foreground_layer[i]);
    }
    free(grid->background_layer);
    free(grid->foreground_layer);
    free(grid);
    SDL_Log("Foreground grid destroyed");
}
/*
 * A háttér fölé kirajzolt réteg kirajzolása a megadott rajzolóra a megadott csempetérképpel.
 * @param renderer a rajzoló
 * @param tilemap a csempetérkép textúrája
 * @param grid a réteg mutatója
 * @param tilemap_width a csempetérkép szélessége
 * @param tilemap_height a csempetérkép magassága
 * @param zoom_level nagyítási szint
 * @param offset_x x eltolás
 * @param offset_y y eltolás
 * @return void
 * @note A réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note A réteg celláinak mérete megegyezik a csempetérkép méretével.
 */
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

/*
 * Az ütközés ellenőrzése adott rács koordinátákon
 * @param grid a rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @return true ha ütközés van, egyébként false
 * @note A rács széleit is ütközésnek tekinti.
 * @note Az ütközés alapja a csempe típusa.
 * @note Az ütközés ellenőrzése az előtér rétegek alapján történik.
 */
bool check_collision(ForegroundGrid *grid, int grid_x, int grid_y)
{
    if (grid_x < 0 || grid_x >= grid->width || grid_y < 0 || grid_y >= grid->height)
    {
        return true; // A pálya széle ütközésnek számít
    }
    return grid->foreground_layer[grid_x][grid_y] != 0; // Az ütközés ellenőrzése az előtér rétegek alapján
}
/*
 * Az előtér beolvasása a megadott fájlból.
 * @param filename a fájl neve
 * @param grid az előtér mutatója
 * @return void
 * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.
 * @note Az előtér réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note Az előtér réteg celláinak mérete megegyezik a csempetérkép méretével.
 * @note Az előtér réteg cellái között nincs rés.
 * @note Az előtér réteg celláinak szélessége és magassága megegyezik a háttér rétegével.
 */
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
/*
 * Az ütközési adatok beolvasása a megadott fájlból.
 * @param filename a fájl neve
 * @param grid a rács mutatója
 * @return void
 * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.
 * @note Az ütközési réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note Az ütközési réteg celláinak mérete megegyezik a csempetérkép méretével.
 * @note Az ütközési réteg cellái között nincs rés.
 * @note Az ütközési réteg celláinak szélessége és magassága megegyezik a háttér rétegével.
 */
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
            fscanf(file, "%2x", &collision);                   // 2 számjegyű hexadecimális szám beolvasása
            grid->collision_layer[j][i] = (collision == 0xFF); // Az ütközési réteg beállítása, FF esetén ütközés van
        }
    }

    fclose(file);
}
/*
 * Ütközési adatok automatikus generálása a háttér és előtér rétegek alapján.
 * @param filename a fájl neve
 * @param grid a háttér réteg mutatója
 * @param fg_grid az előtér réteg mutatója
 * @return void
 * @note Az ütközési réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note Az ütközési réteg celláinak mérete megegyezik a csempetérkép méretével.
 */
void toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y)
{
    if (!grid->collision_layer[grid_x][grid_y]) // Csak 00-t tud FF re váltani
    {
        grid->collision_layer[grid_x][grid_y] = true;

        SDL_Log("Toggling collision at (%d, %d)", grid_x, grid_y); // Átállítás naplózása

        FILE *file = fopen(filename, "r+");
        if (file == NULL)
        {
            SDL_Log("Collision data file error: %s", strerror(errno));
            return;
        }

        int line_length = grid->width * 3;                              // 2 hexadecimális szám + szóköz
        fseek(file, grid_y * (line_length + 1) + grid_x * 3, SEEK_SET); // \n karakterek miatt +1
        fprintf(file, "%02X", 0xFF);

        fclose(file);
    }
}
/*
 * Háttér rács adatainak mentése
 * @param filename a mentési fájl neve
 * @param grid a rács mutatója
 * @return void
 * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.
 */
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
    SDL_Log("Saved grid state to %s", filename); // Mentés naplózása
}
/*
 * Előtér rács adatainak mentése
 * @param filename a mentési fájl neve
 * @param grid az előtér mutatója
 * @return void
 * @note A fájlban a cellák hexadecimális számokkal vannak reprezentálva.
 */
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
/*
    * Előtér rács letisztítása
    * @param grid az előtér mutatója
    * @return void
    * @note Az előtér réteg celláinak értékeit 0-ra állítja.

*/
void clear_foreground_grid(ForegroundGrid *grid)
{
    for (int i = 0; i < grid->width; i++)
    {
        for (int j = 0; j < grid->height; j++)
        {
            grid->foreground_layer[i][j] = 0;
        }
    }
}
/*
 * Ütközési adatok frissítése a háttér és előtér rétegek alapján.
 * @param filename a fájl neve
 * @param grid a háttér réteg mutatója
 * @param fg_grid az előtér réteg mutatója
 * @return void
 * @note Az ütközési réteg cellái a csempetérkép indexeivel vannak reprezentálva.
 * @note Az ütközési réteg celláinak mérete megegyezik a csempetérkép méretével.
 */
void update_collision_data(const char *filename, Grid *grid, ForegroundGrid *fg_grid)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Failed to open collision data file for writing: %s", strerror(errno));
        return;
    }

    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            bool collision = grid->collision_layer[j][i]; // FF megtartása, csak 00-t írunk felül

            // Háttér rács ellenőrzése ütközésre
            for (int k = 0; k < sizeof(background_collision_tiles) / sizeof(int); k++)
            {
                if (grid->physical_layer[j][i] == background_collision_tiles[k])
                {
                    collision = true;
                    break;
                }
            }

            // Előtér rács ellenőrzése ütközésre
            for (int k = 0; k < sizeof(foreground_collision_tiles) / sizeof(int); k++)
            {
                if (fg_grid->foreground_layer[j][i] == foreground_collision_tiles[k])
                {
                    collision = true;
                    break;
                }
            }

            fprintf(file, "%02X ", collision ? 0xFF : (grid->collision_layer[j][i] ? 0xFF : 0x00));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    SDL_Log("Updated collision data in %s", filename); // frissítés naplózása
}
