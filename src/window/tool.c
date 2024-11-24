#include "window.h"
#include <stdlib.h> // stdlib.h a realloc-hoz

/*
 * Kezeli az eszköz műveleteket a megadott rács koordinátákon.
 * @param tool az eszköz típusa
 * @param grid a rács mutatója
 * @param fg_grid az előtér rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param crop_manager a növénykezelő mutatója
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 * @note Az eszköz műveletei: kapálás, öntözés, betakarítás.
 */
void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    // A célzott csempe koordinátáinak használata a karakter lábai helyett
    int target_x = grid_x;
    int target_y = grid_y;

    switch (tool)
    {
    case TOOL_HOE:
        SDL_Log("Kapálás a csempén (%d, %d)", target_x, target_y); // Kapálás naplózása
        set_tile_type(grid, target_x, target_y, TILE_HOE);
        update_tile_texture(grid, target_x, target_y);
        break;
    case TOOL_WATERING_CAN:
        SDL_Log("Öntözés a csempén (%d, %d)", target_x, target_y); // Öntözés naplózása
        if (get_tile_type(grid, target_x, target_y) == TILE_HOE)
        {
            set_tile_type(grid, target_x, target_y, TILE_WATERED);
            update_tile_texture(grid, target_x, target_y);
        }
        break;
    case TOOL_SICKLE:
        SDL_Log("Sarló használata a csempén (%d, %d)", target_x, target_y); // Sarló használatának naplózása
        handle_harvest_action(grid, fg_grid, target_x, target_y, crop_manager, inventory_selection);
        break;
        // További eszköz műveletek hozzáadása
    }
}

/*
 * Kezeli a növény ültetését a megadott rács koordinátákon.
 * @param grid a rács mutatója
 * @param fg_grid az előtér rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param crop_manager a növénykezelő mutatója
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 * @note A növény ültetése csak akkor lehetséges, ha a csempe kapált vagy öntözött, és az előtér üres.
 */
void handle_crop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    int seed_index = inventory_selection->selected_aux_item - 16; // A mag típus indexének kiszámítása
    SeedType seed_type = (SeedType)seed_index;                    // A mag típus közvetlenül az indexből
    CropType crop_type;

    // Mag típus hozzárendelése a növény típushoz
    switch (seed_type)
    {
    case SEED_PARSNIP:
        crop_type = CROP_PARSNIP;
        break;
    case SEED_CAULIFLOWER:
        crop_type = CROP_CAULIFLOWER;
        break;
    case SEED_COFFEE:
        crop_type = CROP_COFFEE;
        break;
    case SEED_GREEN_BEAN:
        crop_type = CROP_GREEN_BEAN;
        break;
    case SEED_HOPS:
        crop_type = CROP_HOPS;
        break;
    case SEED_POTATO:
        crop_type = CROP_POTATO;
        break;
    case SEED_STRAWBERRY:
        crop_type = CROP_STRAWBERRY;
        break;
    case SEED_MELON:
        crop_type = CROP_MELON;
        break;
    case SEED_STARFRUIT:
        crop_type = CROP_STARFRUIT;
        break;
    default:
        SDL_Log("Ismeretlen mag típus: %d", seed_type);
        return;
    }

    SDL_Log("Mag ültetése a típusból %d a (%d, %d) koordinátákra", crop_type, grid_x, grid_y);

    if (inventory_selection->seed_counts[seed_index] > 0) // Ellenőrzi, hogy van-e elérhető mag
    {
        SDL_Log("Magok száma ültetés előtt: %d", inventory_selection->seed_counts[seed_index]);

        if ((get_tile_type(grid, grid_x, grid_y) == TILE_HOE || get_tile_type(grid, grid_x, grid_y) == TILE_WATERED) &&
            fg_grid->foreground_layer[grid_x][grid_y] == 0) // Ellenőrzi, hogy az előtér üres-e
        {
            add_crop(crop_manager, grid_x, grid_y, crop_type, 10); // Példa növekedési idő
            inventory_selection->seed_counts[seed_index]--;        // Csökkenti a magok számát a memóriában
            SDL_Log("Mag elültetve. Új magok száma: %d", inventory_selection->seed_counts[seed_index]);

            // Frissíti az előtér rács állapotát a növény típusával
            fg_grid->foreground_layer[grid_x][grid_y] = crop_info[crop_type].texture_start;

            // Elmenti az előtér rács állapotát a fájlba
            save_foreground_grid_state("../src/foreground_grid_state.txt", fg_grid);
        }
        else
        {
            SDL_Log("Nem lehet magot ültetni. A csempe nem kapált, öntözött, vagy az előtér nem üres.");
        }
    }
    else
    {
        SDL_Log("Nincs elérhető mag az ültetéshez. %d,%d", inventory_selection->seed_counts[seed_index], seed_index);
    }
}

/*
 * Kezeli a növény betakarítását a megadott rács koordinátákon.
 * @param grid a rács mutatója
 * @param fg_grid az előtér rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param crop_manager a növénykezelő mutatója
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 * @note A növény betakarítása csak akkor lehetséges, ha a növény elérte a maximális növekedési fázist.
 */
void handle_harvest_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        if (crop->x == grid_x && crop->y == grid_y && crop->growth_stage == crop_info[crop->type].growth_stages - 1)
        {
            SDL_Log("Növény betakarítása a (%d, %d) koordinátákon", grid_x, grid_y);

            // Hozzáadja a betakarított növényt az inventárhoz
            bool added_to_inventory = false;
            for (int j = 0; j < INVENTORY_SIZE; j++)
            {
                if (inventory_selection->harvest_counts[j] < 256) // MAX 256 elem / slot
                {
                    inventory_selection->harvest_counts[j]++;
                    inventory_selection->seed_types[j] = (SeedType)crop->type;
                    added_to_inventory = true;
                    break;
                }
            }

            if (!added_to_inventory)
            {
                SDL_Log("Az inventár tele van, nem lehet hozzáadni a betakarított növényt.");
                return;
            }

            // Eltávolítja a növényt a növénykezelőből
            crop_manager->crops[i] = crop_manager->crops[crop_manager->crop_count - 1];
            crop_manager->crop_count--;
            crop_manager->crops = realloc(crop_manager->crops, crop_manager->crop_count * sizeof(Crop));

            // Visszaállítja a csempét a kapált fázisra
            set_tile_type(grid, grid_x, grid_y, TILE_HOE);
            update_tile_texture(grid, grid_x, grid_y);

            // Törli az előtér rácsot
            fg_grid->foreground_layer[grid_x][grid_y] = 0;

            break;
        }
    }
}

/*
 * Kezeli a bolt műveleteit a megadott rács koordinátákon.
 * @param grid a rács mutatója
 * @param fg_grid az előtér rács mutatója
 * @param grid_x a rács x koordinátája
 * @param grid_y a rács y koordinátája
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 * @note A bolt műveletei: vetőmag vásárlás, termény eladás.
 */
void handle_shop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, InventorySelection *inventory_selection)
{
    int tile_type = fg_grid->foreground_layer[grid_x][grid_y];
    if (tile_type == 0xD8 || tile_type == 0xD9 || tile_type == 0xE8 || tile_type == 0xE9) // Bolt csempék
    {
        if (inventory_selection->selected_main_item == 3) // Magtáska
        {
            int seed_index = inventory_selection->selected_aux_item - 16;
            SeedType seed_type = (SeedType)seed_index;
            int seed_price = 0;

            switch (seed_type)
            {
            case SEED_PARSNIP:
                seed_price = SEED_PRICE_PARSNIP;
                break;
            case SEED_CAULIFLOWER:
                seed_price = SEED_PRICE_CAULIFLOWER;
                break;
            case SEED_COFFEE:
                seed_price = SEED_PRICE_COFFEE;
                break;
            case SEED_GREEN_BEAN:
                seed_price = SEED_PRICE_GREEN_BEAN;
                break;
            case SEED_HOPS:
                seed_price = SEED_PRICE_HOPS;
                break;
            case SEED_POTATO:
                seed_price = SEED_PRICE_POTATO;
                break;
            case SEED_STRAWBERRY:
                seed_price = SEED_PRICE_STRAWBERRY;
                break;
            case SEED_MELON:
                seed_price = SEED_PRICE_MELON;
                break;
            case SEED_STARFRUIT:
                seed_price = SEED_PRICE_STARFRUIT;
                break;
            default:
                SDL_Log("Ismeretlen mag típus: %d", seed_type);
                return;
            }

            if (inventory_selection->money >= seed_price)
            {
                inventory_selection->seed_counts[seed_index]++;
                inventory_selection->money -= seed_price;
                SDL_Log("Vett 1 magot a típusból %d %d pénzért. Maradék pénz: %d", seed_type, seed_price, inventory_selection->money);
            }
            else
            {
                SDL_Log("Nincs elég pénz a mag vásárlásához a típusból %d", seed_type);
            }
        }
        else if (inventory_selection->selected_main_item == 4) // Betakarított terménytáska
        {
            int total_money = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
                SeedType seed_type = inventory_selection->seed_types[i];
                int seed_count = inventory_selection->harvest_counts[i];
                int crop_price = 0;

                switch (seed_type)
                {
                case SEED_PARSNIP:
                    crop_price = CROP_PRICE_PARSNIP;
                    break;
                case SEED_CAULIFLOWER:
                    crop_price = CROP_PRICE_CAULIFLOWER;
                    break;
                case SEED_COFFEE:
                    crop_price = CROP_PRICE_COFFEE;
                    break;
                case SEED_GREEN_BEAN:
                    crop_price = CROP_PRICE_GREEN_BEAN;
                    break;
                case SEED_HOPS:
                    crop_price = CROP_PRICE_HOPS;
                    break;
                case SEED_POTATO:
                    crop_price = CROP_PRICE_POTATO;
                    break;
                case SEED_STRAWBERRY:
                    crop_price = CROP_PRICE_STRAWBERRY;
                    break;
                case SEED_MELON:
                    crop_price = CROP_PRICE_MELON;
                    break;
                case SEED_STARFRUIT:
                    crop_price = CROP_PRICE_STARFRUIT;
                    break;
                default:
                    SDL_Log("Ismeretlen mag típus: %d", seed_type);
                    continue;
                }

                total_money += seed_count * crop_price;
                inventory_selection->harvest_counts[i] = 0; // Betakarított termények számának törlése
            }

            inventory_selection->money += total_money;
            SDL_Log("Eladta az összes betakarított terményt %d pénzért. Összes pénz: %d", total_money, inventory_selection->money);
        }
    }
}