#include "window.h"

void handle_movement(const bool *state, int *character_x, int *character_y, AnimationController *anim_ctrl, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height)
{
    anim_ctrl->is_walking = false;
    int new_x = *character_x;
    int new_y = *character_y;

    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    {
        new_y -= movement_speed;
        anim_ctrl->direction = DIRECTION_UP;
        anim_ctrl->is_walking = true;
    }
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
    {
        new_y += movement_speed;
        anim_ctrl->direction = DIRECTION_DOWN;
        anim_ctrl->is_walking = true;
    }
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        new_x -= movement_speed;
        anim_ctrl->direction = DIRECTION_LEFT;
        anim_ctrl->is_walking = true;
    }
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        new_x += movement_speed;
        anim_ctrl->direction = DIRECTION_RIGHT;
        anim_ctrl->is_walking = true;
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
    int grid_x = (new_x + character_tile_width / 2) / tile_size;
    int grid_y = (new_y + character_tile_height) / tile_size; // Use character's bottom center for collision detection
    if (grid_x >= 0 && grid_x < grid->width && grid_y >= 0 && grid_y < grid->height)
    {
        if (!grid->collision_layer[grid_x][grid_y])
        {
            *character_x = new_x;
            *character_y = new_y;
            SDL_Log("Player moved to tile (%d, %d)", grid_x, grid_y);
        }
        else
        {
            SDL_Log("Attempted to move to forbidden tile (%d, %d)", grid_x, grid_y);
        }
    }
}