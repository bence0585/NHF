
#include "window.h"

void initialize_character(Character *character, int start_x, int start_y)
{
    character->x = start_x;
    character->y = start_y;
    character->equipped_tool = TOOL_HOE;                                                 // Default tool
    character->anim_ctrl = (AnimationController){0, 6, 10, 0, 1, DIRECTION_DOWN, false}; // Initialize animation controller
    update_character_tile(character, TILE_SIZE);
}

void update_character_tile(Character *character, int tile_size)
{
    convert_to_grid_coordinates(character->x, character->y + tile_size, tile_size, &character->tile_x, &character->tile_y);
}