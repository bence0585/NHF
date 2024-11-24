#include "window.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

void save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Error opening save file: %s", strerror(errno));
        return;
    }
    fprintf(file, "%d %d\n", character_x, character_y);
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_counts[i]); // Use %03d to ensure 3-digit formatting
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->harvest_counts[i]); // Use %03d to ensure 3-digit formatting
    }
    fprintf(file, "\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fprintf(file, "%03d ", inventory_selection->seed_types[i]); // Use %03d to ensure 3-digit formatting
    }
    fprintf(file, "\n%d\n", inventory_selection->total_seeds); // Save total_seeds with newline
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
        save_game_state(filename, *character_x, *character_y, inventory_selection);
        return;
    }
    fscanf(file, "%d %d", character_x, character_y);
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->seed_counts[i]); // Use %03d to ensure 3-digit formatting
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", &inventory_selection->harvest_counts[i]); // Use %03d to ensure 3-digit formatting
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        fscanf(file, "%03d", (int *)&inventory_selection->seed_types[i]); // Use %03d to ensure 3-digit formatting
    }
    fscanf(file, "%d\n", &inventory_selection->total_seeds); // Load total_seeds with newline
    fclose(file);
}