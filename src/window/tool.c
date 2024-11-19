#include "window.h"

void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager)
{
    switch (tool)
    {
    case TOOL_HOE:
        set_tile_type(grid, grid_x, grid_y, TILE_HOE);
        break;
    case TOOL_WATERING_CAN:
        // Implement watering action
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
        add_crop(crop_manager, grid_x, grid_y, 10); // Example growth time
        break;
    case CROP_PRODUCT_1:
    case CROP_PRODUCT_2:
    case CROP_PRODUCT_3:
        // Implement product action
        break;
        // Add more crop actions here
    }
}