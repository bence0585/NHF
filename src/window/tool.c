#include "window.h"

void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager)
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
        on_tile_change(grid, target_x, target_y, TILE_HOE); // Call on_tile_change
        break;
    case TOOL_WATERING_CAN:
        SDL_Log("Watering tile at (%d, %d)", target_x, target_y); // Log the watering action
        if (get_tile_type(grid, target_x, target_y) == TILE_HOE)
        {
            set_tile_type(grid, target_x, target_y, TILE_WATERED);
            update_tile_texture(grid, target_x, target_y);
            on_tile_change(grid, target_x, target_y, TILE_WATERED); // Call on_tile_change
        }
        break;
    case TOOL_SICKLE:
        // Implement cutting action
        break;
        // Add more tool actions here
    }
}

void handle_crop_action(CropType crop, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection)
{
    int seed_index = inventory_selection->selected_aux_item - 16; // Calculate the index for the seed type
    SDL_Log("Attempting to plant seed of type %d at (%d, %d)", crop, grid_x, grid_y);

    if (inventory_selection->seed_counts[seed_index] > 0) // Check if there are seeds available
    {
        SDL_Log("Seed count before planting: %d", inventory_selection->seed_counts[seed_index]);

        if (get_tile_type(grid, grid_x, grid_y) == TILE_HOE || get_tile_type(grid, grid_x, grid_y) == TILE_WATERED)
        {
            add_crop(crop_manager, grid_x, grid_y, 10);     // Example growth time
            inventory_selection->seed_counts[seed_index]--; // Decrement the seed count
            SDL_Log("Seed planted. New seed count: %d", inventory_selection->seed_counts[seed_index]);
            fg_grid->foreground_layer[grid_x][grid_y] = crop;                        // Store the seed in the foreground grid state
            save_foreground_grid_state("../src/foreground_grid_state.txt", fg_grid); // Save changes to the foreground grid state file
        }
        else
        {
            SDL_Log("Cannot plant seed. Tile is not hoed or watered.");
        }
    }
    else
    {
        SDL_Log("No seeds available for planting. %d,%d", inventory_selection->seed_counts[seed_index], seed_index);
    }
}