#include "window.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // Tartalmazza a stdlib.h-t a malloc-hoz
#include "../debugmalloc.h"

/*
 * Mentési fájl létrehozása és a játékállapot mentése.
 * @param filename a mentési fájl neve
 * @param character_x a karakter x koordinátája
 * @param character_y a karakter y koordinátája
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 */
void save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Hiba a mentési fájl megnyitásakor: %s", strerror(errno));
        return;
    }
    fprintf(file, "%d %d\n", character_x, character_y); // 1. sor: karakter koordináták
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_counts[i]); // 2. sor: magok száma
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->harvest_counts[i]); // 3. sor: betakarított termények száma
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_types[i]); // 4. sor: magok típusai
    }
    fprintf(file, "\n%d\n", inventory_selection->total_seeds); // 5. sor: összes mag
    fprintf(file, "%d\n", inventory_selection->money);         // 6. sor: pénz
    fclose(file);
}

/*
 * Betölti a játékállapotot a mentési fájlból.
 * Ha a fájl nem található, új fájlt hoz létre alapértelmezett értékekkel.
 * @param filename a mentési fájl neve
 * @param character_x a karakter x koordinátájának mutatója
 * @param character_y a karakter y koordinátájának mutatója
 * @param inventory_selection az inventárválasztó mutatója
 * @return void
 */
void load_game_state(const char *filename, int *character_x, int *character_y, InventorySelection *inventory_selection)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Mentési fájl nem található, új létrehozása.");
        *character_x = 0;
        *character_y = 0;
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            inventory_selection->seed_counts[i] = 0;
            inventory_selection->harvest_counts[i] = 0;
            inventory_selection->seed_types[i] = (SeedType)i; // Mag típusok inicializálása 0-tól 8-ig
        }
        inventory_selection->total_seeds = 0; // Összes mag inicializálása
        inventory_selection->money = 100;     // Pénz inicializálása
        save_game_state(filename, *character_x, *character_y, inventory_selection);
        return;
    }
    fscanf(file, "%d %d", character_x, character_y); // 1. sor: karakter koordináták
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->seed_counts[i]); // 2. sor: magok száma
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->harvest_counts[i]); // 3. sor: betakarított termények száma
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", (int *)&inventory_selection->seed_types[i]); // 4. sor: magok típusai
    }
    fscanf(file, "%d\n", &inventory_selection->total_seeds); // 5. sor: összes mag
    fscanf(file, "%d\n", &inventory_selection->money);       // 6. sor: pénz
    fclose(file);
}

/*
 * Mentési fájl létrehozása és a növényállapot mentése.
 * @param filename a mentési fájl neve
 * @param crop_manager a növénykezelő mutatója
 * @return void
 */
void save_crop_state(const char *filename, CropManager *crop_manager)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Hiba a növényállapot fájl megnyitásakor: %s", strerror(errno));
        return;
    }

    fprintf(file, "%d\n", crop_manager->crop_count); // Növények számának mentése
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        fprintf(file, "%d %d %d %d %d %d\n", crop->x, crop->y, crop->type, crop->growth_stage, crop->growth_time, crop->current_time);
    }

    fclose(file);
}

/*
 * Betölti a növényállapotot a mentési fájlból.
 * Ha a fájl nem található, új fájlt hoz létre alapértelmezett értékekkel.
 * @param filename a mentési fájl neve
 * @param crop_manager a növénykezelő mutatója
 * @return void
 */
void load_crop_state(const char *filename, CropManager *crop_manager)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Növényállapot fájl nem található, új létrehozása.");
        crop_manager->crop_count = 0;
        crop_manager->crops = NULL;
        save_crop_state(filename, crop_manager);
        return;
    }

    fscanf(file, "%d", &crop_manager->crop_count);
    crop_manager->crops = (Crop *)malloc(crop_manager->crop_count * sizeof(Crop));
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        fscanf(file, "%d %d %d %d %d %d", &crop->x, &crop->y, (int *)&crop->type, (int *)&crop->growth_stage, &crop->growth_time, &crop->current_time);
    }

    fclose(file);
}