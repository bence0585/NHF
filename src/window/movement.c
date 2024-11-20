#include "window.h"

void handle_character_movement(const bool *state, Character *character, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height)
{
    character->anim_ctrl.is_walking = false;
    int new_x = character->x;
    int new_y = character->y;

    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    {
        new_y -= movement_speed;
        character->anim_ctrl.direction = DIRECTION_UP;
        character->anim_ctrl.is_walking = true;
    }
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
    {
        new_y += movement_speed;
        character->anim_ctrl.direction = DIRECTION_DOWN;
        character->anim_ctrl.is_walking = true;
    }
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        new_x -= movement_speed;
        character->anim_ctrl.direction = DIRECTION_LEFT;
        character->anim_ctrl.is_walking = true;
    }
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        new_x += movement_speed;
        character->anim_ctrl.direction = DIRECTION_RIGHT;
        character->anim_ctrl.is_walking = true;
    }

    // Ensure the character cannot exit the grid
    if (new_x < 0)
        new_x = 0;
    if (new_y < 0)
        new_y = 0;
    if (new_x > (grid->width * tile_size - character_tile_width))
        new_x = grid->width * tile_size - character_tile_width;
    if (new_y > (grid->height * tile_size - character_tile_height))
        new_y = grid->height * tile_size - character_tile_height;

    // Check for collisions
    int grid_x, grid_y;
    convert_to_grid_coordinates(new_x + character_tile_width / 2, new_y + character_tile_height / 2, tile_size, &grid_x, &grid_y);
    if (grid_x >= 0 && grid_x < grid->width && grid_y >= 0 && grid_y < grid->height)
    {
        if (!grid->collision_layer[grid_x][grid_y])
        {
            character->x = new_x;
            character->y = new_y;
            // SDL_Log("Player moved to tile (%d, %d)", grid_x, grid_y);
        }
        else
        {
            SDL_Log("Attempted to move to forbidden tile (%d, %d)", grid_x, grid_y);
        }
    }
}