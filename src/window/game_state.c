#include "window.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // Include stdlib.h for malloc

void save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Error opening save file: %s", strerror(errno));
        return;
    }
    fprintf(file, "%d %d\n", character_x, character_y); // Line 1: character coordinates
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_counts[i]); // Line 2: seed counts
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->harvest_counts[i]); // Line 3: harvest counts
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_types[i]); // Line 4: seed types
    }
    fprintf(file, "\n%d\n", inventory_selection->total_seeds); // Line 5: total seeds
    fprintf(file, "%d\n", inventory_selection->money);         // Line 6: money
    fclose(file);
}

void load_game_state(const char *filename, int *character_x, int *character_y, InventorySelection *inventory_selection)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Save file not found, creating a new one.");
        *character_x = 0;
        *character_y = 0;
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            inventory_selection->seed_counts[i] = 0;
            inventory_selection->harvest_counts[i] = 0;
            inventory_selection->seed_types[i] = (SeedType)i; // Initialize seed types from 0 to 8
        }
        inventory_selection->total_seeds = 0; // Initialize total_seeds
        inventory_selection->money = 100;     // Initialize money
        save_game_state(filename, *character_x, *character_y, inventory_selection);
        return;
    }
    fscanf(file, "%d %d", character_x, character_y); // Line 1: character coordinates
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->seed_counts[i]); // Line 2: seed counts
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->harvest_counts[i]); // Line 3: harvest counts
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", (int *)&inventory_selection->seed_types[i]); // Line 4: seed types
    }
    fscanf(file, "%d\n", &inventory_selection->total_seeds); // Line 5: total seeds
    fscanf(file, "%d\n", &inventory_selection->money);       // Line 6: money
    fclose(file);
}

void save_crop_state(const char *filename, CropManager *crop_manager)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Error opening crop state file: %s", strerror(errno));
        return;
    }

    fprintf(file, "%d\n", crop_manager->crop_count); // Save the number of crops
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        fprintf(file, "%d %d %d %d %d %d\n", crop->x, crop->y, crop->type, crop->growth_stage, crop->growth_time, crop->current_time);
    }

    fclose(file);
}

void load_crop_state(const char *filename, CropManager *crop_manager)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Crop state file not found, creating a new one.");
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