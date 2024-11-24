#include "window.h"
/*
 * Inicializálja a karaktert a megadott kezdőpozícióval.
 * A karakter alapértelmezett eszköze a kapa.
 * Az animációvezérlő inicializálása.
 * @param character karakter mutatója
 * @param start_x kezdő x pozíció
 * @param start_y kezdő y pozíció
 * @return void
 */
void initialize_character(Character *character, int start_x, int start_y)
{
    character->x = start_x;
    character->y = start_y;
    character->equipped_tool = TOOL_HOE;                                                 // Default tool
    character->anim_ctrl = (AnimationController){0, 6, 10, 0, 1, DIRECTION_DOWN, false}; // Initialize animation controller
    update_character_tile(character, TILE_SIZE);
    calculate_look_coordinates(character, TILE_SIZE);
}
/*
 * Frissíti a karakter csempe koordinátáit a karakter x és y pozíciója alapján.
 * @param character karakter mutatója
 * @param tile_size csempe mérete
 * @return void
 */
void update_character_tile(Character *character, int tile_size)
{
    convert_to_grid_coordinates(character->x, character->y + tile_size / 2, tile_size, &character->tile_x, &character->tile_y);
}
/*
 * Kezeli a karakter mozgását a megadott állapotok alapján.
 * A karakter mozgási sebessége a movement_speed paraméterrel állítható.
 * A karakter nem hagyhatja el a rácsot.
 * Ellenőrzi az ütközéseket.
 * @param state billentyűállapotok
 * @param character karakter mutatója
 * @param grid rács mutatója
 * @param movement_speed mozgási sebesség
 * @param tile_size csempe mérete
 * @param character_tile_width karakter csempe szélessége
 * @param character_tile_height karakter csempe magassága
 * @return void
 */
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
            update_character_tile(character, tile_size);
            calculate_look_coordinates(character, tile_size);
            // SDL_Log("Player moved to tile (%d, %d)", grid_x, grid_y);
        }
        else
        {
            SDL_Log("Attempted to move to forbidden tile (%d, %d)", grid_x, grid_y);
        }
    }
}
/*
 * Kiszámítja a karakter nézési koordinátáit. A karakter nézési iránya alapján.
 * @param character karakter mutatója
 * @param tile_size csempe mérete
 * @return void
 */
void calculate_look_coordinates(Character *character, int tile_size)
{
    character->look_x = character->x;
    character->look_y = character->y;
    switch (character->anim_ctrl.direction)
    {
    case DIRECTION_UP:
        character->look_y -= tile_size;
        break;
    case DIRECTION_DOWN:
        character->look_y += tile_size;
        break;
    case DIRECTION_LEFT:
        character->look_x -= tile_size;
        break;
    case DIRECTION_RIGHT:
        character->look_x += tile_size;
        break;
    }
    update_look_tile(character, tile_size);
}
/*
 * Frissíti a karakter nézési csempéjét a karakter nézési koordinátái alapján.
 * @param character karakter mutatója
 * @param tile_size csempe mérete
 * @return void
 */
void update_look_tile(Character *character, int tile_size)
{
    convert_to_grid_coordinates(character->look_x, character->look_y + tile_size / 2, tile_size, &character->look_tile_x, &character->look_tile_y);
}
/*
 * Frissíti az animációt az animációvezérlő alapján.
 * Az animációvezérlő frame_delay_counter értékét növeli.
 * Az animációvezérlő frame_delay értékével összehasonlítja a frame_delay_counter-t.
 * Ha a frame_delay_counter kisebb, mint a frame_delay, akkor visszatér.
 * Az animációvezérlő frame_delay_counter értékét nullázza.
 * Ha a karakter mozog, akkor a frame értékét növeli a max_frames értékével és a 3-al vett maradékával.
 * Ha a karakter nem mozog, akkor a frame értékét nullázza.
 * @param anim_ctrl animációvezérlő mutatója
 * @return void
 * @note A karakter mozgásának animációja például: 0-1-2-0-1-2-0-1-2
 * @note A karakter álló animációja: 0-0-0-0
 */
void update_animation(AnimationController *anim_ctrl)
{
    anim_ctrl->frame_delay_counter++;
    if (anim_ctrl->frame_delay_counter < anim_ctrl->frame_delay)
    {
        return;
    }

    anim_ctrl->frame_delay_counter = 0;

    if (anim_ctrl->is_walking)
    {
        anim_ctrl->frame = anim_ctrl->max_frames + (anim_ctrl->frame + 1) % 3; // Walking frames
        return;
    }

    // Idle animation: 0-0-0-0
    anim_ctrl->frame = 0;
}