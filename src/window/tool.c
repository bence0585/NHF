#include "window.h"
#include <stdlib.h> // Include stdlib.h for realloc

void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    // Use the targeted tile coordinates instead of the character's feet
    int target_x = grid_x;
    int target_y = grid_y;

    switch (tool)
    {
    case TOOL_HOE:
        SDL_Log("Hoeing tile at (%d, %d)", target_x, target_y); // Log the hoeing action
        set_tile_type(grid, target_x, target_y, TILE_HOE);
        update_tile_texture(grid, target_x, target_y);
        break;
    case TOOL_WATERING_CAN:
        SDL_Log("Watering tile at (%d, %d)", target_x, target_y); // Log the watering action
        if (get_tile_type(grid, target_x, target_y) == TILE_HOE)
        {
            set_tile_type(grid, target_x, target_y, TILE_WATERED);
            update_tile_texture(grid, target_x, target_y);
        }
        break;
    case TOOL_SICKLE:
        SDL_Log("Using sickle at (%d, %d)", target_x, target_y); // Log the sickle action
        handle_harvest_action(grid, fg_grid, target_x, target_y, crop_manager, inventory_selection);
        break;
        // Add more tool actions here
    }
}

void handle_crop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    int seed_index = inventory_selection->selected_aux_item - 16; // Calculate the index for the seed type
    SeedType seed_type = (SeedType)seed_index;                    // Get the seed type directly from the index
    CropType crop_type;

    // Map seed type to crop type
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
        SDL_Log("Unknown seed type: %d", seed_type);
        return;
    }

    SDL_Log("Attempting to plant seed of type %d at (%d, %d)", crop_type, grid_x, grid_y);

    if (inventory_selection->seed_counts[seed_index] > 0) // Check if there are seeds available
    {
        SDL_Log("Seed count before planting: %d", inventory_selection->seed_counts[seed_index]);

        if ((get_tile_type(grid, grid_x, grid_y) == TILE_HOE || get_tile_type(grid, grid_x, grid_y) == TILE_WATERED) &&
            fg_grid->foreground_layer[grid_x][grid_y] == 0) // Check if the foreground is clear
        {
            add_crop(crop_manager, grid_x, grid_y, crop_type, 1); // Example growth time
            inventory_selection->seed_counts[seed_index]--;       // Decrement the seed count in memory
            SDL_Log("Seed planted. New seed count: %d", inventory_selection->seed_counts[seed_index]);

            // Update the foreground grid state with the crop type
            fg_grid->foreground_layer[grid_x][grid_y] = crop_info[crop_type].texture_start;

            // Do not save the foreground grid state to the file here
        }
        else
        {
            SDL_Log("Cannot plant seed. Tile is not hoed, watered, or the foreground is not clear.");
        }
    }
    else
    {
        SDL_Log("No seeds available for planting. %d,%d", inventory_selection->seed_counts[seed_index], seed_index);
    }
}

void handle_harvest_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        if (crop->x == grid_x && crop->y == grid_y && crop->growth_stage == crop_info[crop->type].growth_stages - 1)
        {
            SDL_Log("Harvesting crop at (%d, %d)", grid_x, grid_y);

            // Add the harvested crop to the inventory
            bool added_to_inventory = false;
            for (int j = 0; j < INVENTORY_SIZE; j++)
            {
                if (inventory_selection->harvest_counts[j] == 0)
                {
                    inventory_selection->harvest_counts[j] = 1;
                    inventory_selection->seed_types[j] = (SeedType)crop->type;
                    added_to_inventory = true;
                    break;
                }
                else if (inventory_selection->seed_types[j] == (SeedType)crop->type)
                {
                    inventory_selection->harvest_counts[j]++;
                    added_to_inventory = true;
                    break;
                }
            }

            if (!added_to_inventory)
            {
                SDL_Log("Inventory full, could not add harvested crop.");
                return;
            }

            // Remove the crop from the crop manager
            crop_manager->crops[i] = crop_manager->crops[crop_manager->crop_count - 1];
            crop_manager->crop_count--;
            crop_manager->crops = realloc(crop_manager->crops, crop_manager->crop_count * sizeof(Crop));

            // Clear the tile back to the hoed phase
            set_tile_type(grid, grid_x, grid_y, TILE_HOE);
            update_tile_texture(grid, grid_x, grid_y);

            // Clear the foreground grid
            fg_grid->foreground_layer[grid_x][grid_y] = 0;

            break;
        }
    }
}