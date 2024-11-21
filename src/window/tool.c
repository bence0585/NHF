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
        set_tile_type(grid, target_x, target_y, TILE_WATERED);
        update_tile_texture(grid, target_x, target_y);
        on_tile_change(grid, target_x, target_y, TILE_WATERED); // Call on_tile_change
        break;
    case TOOL_SICKLE:
        // Implement cutting action
        break;
        // Add more tool actions here
    }
}

void handle_crop_action(CropType crop, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager)
{
    switch (crop)
    {
    case CROP_SEED_1:
    case CROP_SEED_2:
    case CROP_SEED_3:
        add_crop(crop_manager, grid_x, grid_y, 10);                              // Example growth time
        save_foreground_grid_state("../src/foreground_grid_state.txt", fg_grid); // Save changes to the foreground grid state file
        break;
    case CROP_PRODUCT_1:
    case CROP_PRODUCT_2:
    case CROP_PRODUCT_3:
        // Implement product action
        break;
        // Add more crop actions here
    }
}