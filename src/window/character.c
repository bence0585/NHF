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
    character->equipped_tool = TOOL_HOE;                                                 // Alapértelmezett eszköz
    character->anim_ctrl = (AnimationController){0, 6, 10, 0, 1, DIRECTION_DOWN, false}; // Animációvezérlő inicializálása
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

    // Biztosítja, hogy a karakter ne hagyja el a rácsot
    if (new_x < 0)
        new_x = 0;
    if (new_y < 0)
        new_y = 0;
    if (new_x > (grid->width * tile_size - character_tile_width))
        new_x = grid->width * tile_size - character_tile_width;
    if (new_y > (grid->height * tile_size - character_tile_height))
        new_y = grid->height * tile_size - character_tile_height;

    // Ütközések ellenőrzése
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
        }
        else
        {
            SDL_Log("Megkísérelt lépés a tiltott csempére (%d, %d)", grid_x, grid_y);
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
        anim_ctrl->frame = anim_ctrl->max_frames + (anim_ctrl->frame + 1) % 3; // Mozgási frame-ek
        return;
    }

    // Álló animáció: 0-0-0-0
    anim_ctrl->frame = 0;
}

void handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info)
{
    if (event->key.key == SDLK_ESCAPE)
    {
        SDL_Log("Kilepes: SDL3 ESC gomb");
        *quit = true;
        save_game_state("../src/save_state.txt", character->x, character->y, inventory_selection);
        save_grid_state("../src/grid_state.txt", grid);
        save_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid); // Elmenti az előtér rács állapotát (Az előtér tárolja a növényeket, boltot, és minden olyat ami a háttér előtt van)
        save_crop_state("../src/crop_state.txt", crop_manager);                          // Elmenti a növények állapotát
        update_collision_data("../src/collisions.txt", grid, foreground_grid);           // Frissíti az ütközési adatokat
    }
    else if (event->key.key >= SDLK_1 && event->key.key <= SDLK_9)
    {
        if (SDL_GetModState() & SDL_KMOD_SHIFT)
        {
            if (inventory_selection->selected_aux_inventory == 1) // Magtár
            {
                inventory_selection->selected_aux_item = 16 + (event->key.key - SDLK_1);
            }
            else if (inventory_selection->selected_aux_inventory == 2) // Betakarított terménytár
            {
                inventory_selection->selected_aux_item = 32 + (event->key.key - SDLK_1);
            }
        }
        else
        {
            inventory_selection->selected_main_item = event->key.key - SDLK_1;
            if (inventory_selection->selected_main_item >= INVENTORY_SIZE)
            {
                inventory_selection->selected_main_item = INVENTORY_SIZE - 1;
            }
            if (inventory_selection->selected_main_item < 3) // Eszköz
            {
                character->equipped_tool = (ToolType)inventory_selection->selected_main_item;
            }
            if (inventory_selection->selected_main_item == 3) // magtár
            {
                inventory_selection->selected_aux_inventory = 1;
                inventory_selection->selected_aux_item = 16; // Magtár első helyére állít
            }
            else if (inventory_selection->selected_main_item == 4) // Betakarított terménytár
            {
                inventory_selection->selected_aux_inventory = 2;
                inventory_selection->selected_aux_item = 32; // Betakarított terménytár első helyére állít
            }
            else
            {
                inventory_selection->selected_aux_inventory = 0; // Nincs aux tároló
            }
        }
    }
    else if (event->key.key == SDLK_C)
    {
        if (inventory_selection->selected_main_item < 3) // eszköz művelet
        {
            handle_tool_action(character->equipped_tool, grid, foreground_grid, character->look_tile_x, character->look_tile_y, crop_manager, inventory_selection);
        }
        else // ültetés, betakarítás
        {
            handle_crop_action(grid, foreground_grid, character->look_tile_x, character->look_tile_y, crop_manager, inventory_selection);
        }
    }
    else if (event->key.key == SDLK_X)
    {
        if (inventory_selection->selected_main_item == 3 || inventory_selection->selected_main_item == 4) // Bolt művelet
        {
            handle_shop_action(grid, foreground_grid, character->look_tile_x, character->look_tile_y, inventory_selection);
        }
    }
    else if (event->key.key == SDLK_L) // Ütközési adatok módosítása (debug)
    {
        toggle_collision_data("../src/collisions.txt", grid, character->tile_x, character->tile_y);
        read_collision_data("../src/collisions.txt", grid); // Olvassa be az ütközési adatokat
    }
    else if (event->key.key == SDLK_F2)
    {
        *show_debug_info = !*show_debug_info; // Debug információk megjelenítése
    }
}

void handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection)
{
    if (SDL_GetModState() & SDL_KMOD_SHIFT)
    {
        if (inventory_selection->selected_aux_inventory == 1) // magtár
        {
            inventory_selection->selected_aux_item = 16 + (inventory_selection->selected_aux_item - 16 + (event->wheel.y > 0 ? -1 : 1) + INVENTORY_SIZE) % INVENTORY_SIZE;
        }
        else if (inventory_selection->selected_aux_inventory == 2) // betakarított terménytár
        {
            inventory_selection->selected_aux_item = 32 + (inventory_selection->selected_aux_item - 32 + (event->wheel.y > 0 ? -1 : 1) + INVENTORY_SIZE) % INVENTORY_SIZE;
        }
    }
    else
    {
        if (event->wheel.y > 0) // Felgörget
        {
            inventory_selection->selected_main_item = (inventory_selection->selected_main_item - 1 + INVENTORY_SIZE) % INVENTORY_SIZE;
        }
        else if (event->wheel.y < 0) // legörget
        {
            inventory_selection->selected_main_item = (inventory_selection->selected_main_item + 1) % INVENTORY_SIZE;
        }
        if (inventory_selection->selected_main_item == 3) // magtár
        {
            inventory_selection->selected_aux_inventory = 1;
        }
        else if (inventory_selection->selected_main_item == 4) // betakarított terménytár
        {
            inventory_selection->selected_aux_inventory = 2;
        }
        else
        {
            inventory_selection->selected_aux_inventory = 0;
        }
    }
}

void handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level)
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {
        int x = event->button.x;
        int y = event->button.y;
        int slot;
        bool button_clicked = false;

        if (is_button_clicked(BUTTON_ZOOM_IN, x, y))
        {
            if (*zoom_level < 4)
            {
                (*zoom_level)++;
                button_clicked = true;
            }
        }
        else if (is_button_clicked(BUTTON_ZOOM_OUT, x, y))
        {
            if (*zoom_level > 1)
            {
                (*zoom_level)--;
                button_clicked = true;
            }
        }
        else if (is_button_clicked(BUTTON_SAVE_GAME, x, y))
        {
            save_game_state("../src/save_state.txt", character->x, character->y, inventory_selection);
            save_grid_state("../src/grid_state.txt", grid);
            save_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid); // Elmenti az előtér rács állapotát (Az előtér tárolja a növényeket, boltot, és minden olyat ami a háttér előtt van)
            save_crop_state("../src/crop_state.txt", crop_manager);                          // Elmenti a növények állapotát
            update_collision_data("../src/collisions.txt", grid, foreground_grid);           // Frissíti az ütközési adatokat
            button_clicked = true;
        }

        if (!button_clicked)
        {
            if (is_inventory_slot_clicked(x, y, screen_width, screen_height, &slot))
            {
                if (SDL_GetModState() & SDL_KMOD_SHIFT)
                {
                    if (inventory_selection->selected_aux_inventory == 1)
                    {
                        inventory_selection->selected_aux_item = slot + 16; // Magok offsetje, mivel a magtár 16-tól kezdődik a karakterládában
                    }
                    else if (inventory_selection->selected_aux_inventory == 2)
                    {
                        inventory_selection->selected_aux_item = slot + 32; // Betakarított termények offsetje, mivel a betakarított terménytár 32-től kezdődik a karakterládában
                    }
                }
                else
                {
                    inventory_selection->selected_main_item = slot;
                    if (inventory_selection->selected_main_item == 3)
                    {
                        inventory_selection->selected_aux_inventory = 1;
                    }
                    else if (inventory_selection->selected_main_item == 4)
                    {
                        inventory_selection->selected_aux_inventory = 2;
                    }
                    else
                    {
                        inventory_selection->selected_aux_inventory = 0;
                    }
                }
            }
            else
            {
                if (inventory_selection->selected_main_item < 3) // eszköz művelet
                {
                    character->equipped_tool = (ToolType)inventory_selection->selected_main_item;
                    handle_tool_action(character->equipped_tool, grid, foreground_grid, character->look_tile_x, character->look_tile_y, crop_manager, inventory_selection);
                }
                else // ültetés
                {
                    handle_crop_action(grid, foreground_grid, character->look_tile_x, character->look_tile_y, crop_manager, inventory_selection);
                }
            }
        }
    }
}

void update_character_position(Character *character, Grid *grid, int grid_width, int grid_height, int tile_size, int character_tile_width, int character_tile_height)
{
    // Biztosítja, hogy a karakter ne hagyja el a rácsot
    if (character->x < 0)
        character->x = 0;
    if (character->y < 0)
        character->y = 0;
    if (character->x > (grid_width * tile_size - character_tile_width))
        character->x = grid_width * tile_size - character_tile_width;
    if (character->y > (grid_height * tile_size - character_tile_height))
        character->y = grid_height * tile_size - character_tile_height;
}