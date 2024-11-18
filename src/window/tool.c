
#include "window.h"

void handle_tool_action(ToolType tool, Grid *grid, int grid_x, int grid_y, CropManager *crop_manager)
{
    switch (tool)
    {
    case TOOL_HOE:
        set_tile_type(grid, grid_x, grid_y, TILE_HOE);
        add_crop(crop_manager, grid_x, grid_y, 10); // Example growth time
        break;
        // Add more tool actions here
    }
}