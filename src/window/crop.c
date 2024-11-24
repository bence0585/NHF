#include "window.h"
#include <stdlib.h>
#include <time.h>

/*
 * Inicializálja a növénykezelőt. A növények tömbjét NULL-ra állítja, a növények számát pedig 0-ra.
 * @param crop_manager növénykezelő mutatója
 * @return void
 */
void initialize_crop_manager(CropManager *crop_manager)
{
    crop_manager->crops = NULL;
    crop_manager->crop_count = 0;
    srand(time(NULL)); // Inicializálja a véletlenszám generátort
}

/*
 * Hozzáad egy növényt a növénykezelőhöz. A növények tömbjének méretét növeli, és hozzáadja a növényt a tömbhöz.
 * @param crop_manager növénykezelő mutatója
 * @param x x koordináta
 * @param y y koordináta
 * @param type növény típusa
 * @param growth_time növekedési idő
 * @return void
 */
void add_crop(CropManager *crop_manager, int x, int y, CropType type, int growth_time)
{
    crop_manager->crop_count++;
    crop_manager->crops = realloc(crop_manager->crops, crop_manager->crop_count * sizeof(Crop));
    crop_manager->crops[crop_manager->crop_count - 1] = (Crop){x, y, type, CROP_PHASE_1, growth_time, 0};
}

/*
 * Frissíti a növények állapotát a megadott időegységek alapján. A növények növekedési esélye a víz állapotától függ.
 * @param crop_manager növénykezelő mutatója
 * @param ticks időegységek száma
 * @param grid rács mutatója
 * @return void
 * @note A növények növekedési esélye 80%, ha öntözött, 30%, ha nem öntözött.
 */
void update_crops(CropManager *crop_manager, int ticks, Grid *grid)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        int growth_chance = (get_tile_type(grid, crop->x, crop->y) == TILE_WATERED) ? 80 : 30; // olyasmi mint a minecraft randomtickspeed, nem minden tick affektálja a növési fázist az esélyek:( 80%, ha öntözött, 30%, ha nem)

        if (rand() % 100 < growth_chance) // Esély a növekedésre
        {
            crop->current_time += ticks;
            if (crop->current_time >= crop->growth_time && crop->growth_stage < crop_info[crop->type].growth_stages - 1)
            {
                crop->growth_stage++;
                crop->current_time = 0;
            }
            if (crop->growth_stage == crop_info[crop->type].growth_stages - 1)
            {
                crop->current_time = 0; // Biztosítja, hogy az utolsó növekedési fázis a maximális fázis mínusz egy legyen, 0-ás aktuális idővel
            }
        }
    }
}

/*
 * Rendereli a növényeket a megadott paraméterek alapján.
 * @param renderer rajzoló mutatója
 * @param crop_texture növény textúra
 * @param crop_manager növénykezelő mutatója
 * @param tile_size csempe mérete
 * @param zoom_level nagyítási szint
 * @param offset_x x eltolás
 * @param offset_y y eltolás
 * @return void
 * @note A növények mindig 2 magasak.
 */
void render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        int crop_height = 2 * tile_size * zoom_level; // 2 magas a növény
        SDL_FRect dest = {offset_x + crop->x * tile_size * zoom_level, offset_y + (crop->y - 1) * tile_size * zoom_level, tile_size * zoom_level, crop_height};

        const CropInfo *info = &crop_info[crop->type];
        int growth_stage = (crop->growth_stage >= info->growth_stages ? info->growth_stages - 1 : crop->growth_stage);
        int src_x = ((info->texture_start % 16) + growth_stage) * tile_size;
        int src_y = (info->texture_start / 16) * tile_size - tile_size;

        SDL_FRect src = {src_x, src_y, tile_size, 2 * tile_size}; // mivel 2 magas, a forrás textúra 16x32
        SDL_RenderTexture(renderer, crop_texture, &src, &dest);
    }
}