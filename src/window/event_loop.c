#include <stdio.h>
#include <math.h>
#include <SDL3/SDL_render.h> // Include the correct header for SDL_RenderDrawPoint
#include "window.h"

void render_shadow(SDL_Renderer *renderer, int x, int y, int radius)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64); // Semi-transparent black

    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderPoint(renderer, x + dx, y + dy);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void game_tick(CropManager *crop_manager, int ticks)
{
    update_crops(crop_manager, ticks);
}

void event_loop(SDL_Renderer *renderer, Grid *background_grid, ForegroundGrid *foreground_grid)
{
    SDL_Event event;
    int quit = 0;
    int zoom_level = 1;
    const int tilemap_width = 16;
    const int tilemap_height = tilemap_width;
    const int character_tile_width = tilemap_width;
    const int character_tile_height = tilemap_width * 2;
    int tile_size = tilemap_width;

    Character character;
    initialize_character(&character, 0, 0); // Initialize character at top-left tile

    InventorySelection inventory_selection = {0, 0, 0}; // Initialize inventory selection
    load_game_state("../src/save_state.txt", &character.x, &character.y, &inventory_selection);
    update_character_tile(&character, tilemap_width);

    SDL_Texture *tilemap = load_texture(renderer, "../src/img/tilemap.png");
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    SDL_Texture *character_tileset = load_texture(renderer, "../src/img/playerTilemap.png");
    SDL_SetTextureScaleMode(character_tileset, SDL_SCALEMODE_NEAREST);
    SDL_Texture *item_tilemap = load_texture(renderer, "../src/img/itemTilemap.png");
    SDL_SetTextureScaleMode(item_tilemap, SDL_SCALEMODE_NEAREST);
    SDL_Texture *crop_texture = load_texture(renderer, "../src/img/cropTilemap.png"); // Crop texture
    SDL_SetTextureScaleMode(crop_texture, SDL_SCALEMODE_NEAREST);

    int grid_width, grid_height;
    determine_grid_size("../src/grid_state.txt", &grid_width, &grid_height);
    Grid *grid = create_grid(grid_width, grid_height);
    read_grid_state("../src/grid_state.txt", grid);
    read_collision_data("../src/collisions.txt", grid); // Read collision data
    if (grid == NULL)
    {
        SDL_Log("Failed to read grid state.");
        return;
    }

    CropManager crop_manager;
    initialize_crop_manager(&crop_manager);

    Uint32 start_time = SDL_GetTicks();
    int frame_count = 0;
    int fps = 0;

    static const int movement_speed = 2;

    int screen_width, screen_height;
    SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);

    int grid_x, grid_y; // Declare grid_x and grid_y here

    bool show_debug_info = false; // Add a flag to toggle debug info

    while (!quit)
    {
        Uint32 frame_start = SDL_GetTicks();
        frame_count++;
        if (frame_start - start_time >= 1000)
        {
            fps = frame_count;
            frame_count = 0;
            start_time = frame_start;
            game_tick(&crop_manager, 1); // Advance the game by one tick every second
        }

        SDL_PumpEvents();
        const bool *state = SDL_GetKeyboardState(NULL);

        update_character_tile(&character, tilemap_width);

        convert_to_grid_coordinates(character.x + character_tile_width / 2, character.y + character_tile_height / 2, tilemap_width, &grid_x, &grid_y);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                SDL_Log("Kilepes: SDL3 event");
                quit = 1;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    SDL_Log("Kilepes: SDL3 ESC gomb");
                    quit = 1;
                    save_game_state("../src/save_state.txt", character.x, character.y, &inventory_selection);
                    save_grid_state("../src/grid_state.txt", grid);
                    save_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid);
                }
                else if (event.key.key >= SDLK_1 && event.key.key <= SDLK_9)
                {
                    if (SDL_GetModState() & SDL_KMOD_SHIFT)
                    {
                        if (inventory_selection.selected_aux_inventory == 1) // Seed pouch
                        {
                            inventory_selection.selected_aux_item = 16 + (event.key.key - SDLK_1);
                        }
                        else if (inventory_selection.selected_aux_inventory == 2) // Harvest bag
                        {
                            inventory_selection.selected_aux_item = 32 + (event.key.key - SDLK_1);
                        }
                    }
                    else
                    {
                        inventory_selection.selected_main_item = event.key.key - SDLK_1;
                        if (inventory_selection.selected_main_item >= INVENTORY_SIZE)
                        {
                            inventory_selection.selected_main_item = INVENTORY_SIZE - 1;
                        }
                        if (inventory_selection.selected_main_item < 3) // Update equipped tool based on selected item
                        {
                            character.equipped_tool = (ToolType)inventory_selection.selected_main_item;
                        }
                        if (inventory_selection.selected_main_item == 3) // Seed pouch
                        {
                            inventory_selection.selected_aux_inventory = 1;
                        }
                        else if (inventory_selection.selected_main_item == 4) // Harvest bag
                        {
                            inventory_selection.selected_aux_inventory = 2;
                        }
                        else
                        {
                            inventory_selection.selected_aux_inventory = 0;
                        }
                    }
                }
                else if (event.key.key == SDLK_C)
                {
                    handle_tool_action(character.equipped_tool, grid, foreground_grid, character.look_tile_x, character.look_tile_y, &crop_manager);
                }
                else if (event.key.key == SDLK_L)
                {
                    toggle_collision_data("../src/collisions.txt", grid, character.tile_x, character.tile_y);
                    read_collision_data("../src/collisions.txt", grid); // Read collision data again
                }
                else if (event.key.key == SDLK_F2)
                {
                    show_debug_info = !show_debug_info; // Toggle debug info
                }
                break;

            case SDL_EVENT_MOUSE_WHEEL:
                if (SDL_GetModState() & SDL_KMOD_SHIFT)
                {
                    if (inventory_selection.selected_aux_inventory == 1) // Seed pouch
                    {
                        inventory_selection.selected_aux_item = 16 + (inventory_selection.selected_aux_item - 16 + (event.wheel.y > 0 ? -1 : 1) + INVENTORY_SIZE) % INVENTORY_SIZE;
                    }
                    else if (inventory_selection.selected_aux_inventory == 2) // Harvest bag
                    {
                        inventory_selection.selected_aux_item = 32 + (inventory_selection.selected_aux_item - 32 + (event.wheel.y > 0 ? -1 : 1) + INVENTORY_SIZE) % INVENTORY_SIZE;
                    }
                }
                else
                {
                    if (event.wheel.y > 0) // Scroll up
                    {
                        inventory_selection.selected_main_item = (inventory_selection.selected_main_item - 1 + INVENTORY_SIZE) % INVENTORY_SIZE;
                    }
                    else if (event.wheel.y < 0) // Scroll down
                    {
                        inventory_selection.selected_main_item = (inventory_selection.selected_main_item + 1) % INVENTORY_SIZE;
                    }
                    if (inventory_selection.selected_main_item == 3) // Seed pouch
                    {
                        inventory_selection.selected_aux_inventory = 1;
                    }
                    else if (inventory_selection.selected_main_item == 4) // Harvest bag
                    {
                        inventory_selection.selected_aux_inventory = 2;
                    }
                    else
                    {
                        inventory_selection.selected_aux_inventory = 0;
                    }
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    int slot;
                    bool button_clicked = false;

                    if (is_button_clicked(BUTTON_ZOOM_IN, x, y))
                    {
                        if (zoom_level < 4)
                        {
                            zoom_level += 1;
                            button_clicked = true;
                        }
                    }
                    else if (is_button_clicked(BUTTON_ZOOM_OUT, x, y))
                    {
                        if (zoom_level > 1)
                        {
                            zoom_level -= 1;
                            button_clicked = true;
                        }
                    }
                    else if (is_button_clicked(BUTTON_SAVE_GAME, x, y))
                    {
                        save_game_state("../src/save_state.txt", character.x, character.y, &inventory_selection);
                        save_grid_state("../src/grid_state.txt", grid);
                        save_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid);
                        button_clicked = true;
                    }

                    if (!button_clicked)
                    {
                        if (is_inventory_slot_clicked(x, y, screen_width, screen_height, &slot))
                        {
                            if (SDL_GetModState() & SDL_KMOD_SHIFT)
                            {
                                if (inventory_selection.selected_aux_inventory == 1)
                                {
                                    inventory_selection.selected_aux_item = slot + 16; // Offset for seeds
                                }
                                else if (inventory_selection.selected_aux_inventory == 2)
                                {
                                    inventory_selection.selected_aux_item = slot + 32; // Offset for harvested products
                                }
                            }
                            else
                            {
                                inventory_selection.selected_main_item = slot;
                                if (inventory_selection.selected_main_item == 3) // Seed pouch
                                {
                                    inventory_selection.selected_aux_inventory = 1;
                                }
                                else if (inventory_selection.selected_main_item == 4) // Harvest bag
                                {
                                    inventory_selection.selected_aux_inventory = 2;
                                }
                                else
                                {
                                    inventory_selection.selected_aux_inventory = 0;
                                }
                            }
                        }
                        else
                        {
                            if (inventory_selection.selected_main_item < 3) // Update equipped tool based on selected item
                            {
                                character.equipped_tool = (ToolType)inventory_selection.selected_main_item;
                                handle_tool_action(character.equipped_tool, grid, foreground_grid, character.look_tile_x, character.look_tile_y, &crop_manager);
                            }
                            else
                            {
                                handle_crop_action((CropType)(inventory_selection.selected_main_item - 3), grid, foreground_grid, character.look_tile_x, character.look_tile_y, &crop_manager);
                            }
                        }
                        if (inventory_selection.selected_main_item < 3) // Update equipped tool based on selected item
                        {
                            character.equipped_tool = (ToolType)inventory_selection.selected_main_item;
                            handle_tool_action(character.equipped_tool, grid, foreground_grid, character.look_tile_x, character.look_tile_y, &crop_manager);
                        }
                    }
                }
            }
        }

        handle_character_movement(state, &character, grid, movement_speed, tile_size, character_tile_width, character_tile_height);
        calculate_look_coordinates(&character, tile_size); // Ensure look coordinates are updated

        // Ensure the character cannot exit the grid
        if (character.x < 0)
            character.x = 0;
        if (character.y < 0)
            character.y = 0;
        if (character.x > (grid_width * tile_size - character_tile_width))
            character.x = grid_width * tile_size - character_tile_width;
        if (character.y > (grid_height * tile_size - character_tile_height))
            character.y = grid_height * tile_size - character_tile_height;

        // Log the new tile position
        update_character_tile(&character, tile_size);
        calculate_look_coordinates(&character, tile_size); // Ensure look coordinates are updated
        // SDL_Log("Player is on tile (%d, %d)", character.tile_x, character.tile_y);

        update_animation(&character.anim_ctrl);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character.x * zoom_level - (tilemap_width * zoom_level) / 2;
        int offset_y = screen_height / 2 - character.y * zoom_level - (tilemap_height * zoom_level) / 2;

        render_grid(renderer, tilemap, grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);

        render_foreground_grid(renderer, crop_texture, foreground_grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);

        highlight_grid_square(renderer, character.tile_x, character.tile_y, tilemap_width, zoom_level, offset_x, offset_y);
        highlight_look_square(renderer, character.look_tile_x, character.look_tile_y, tilemap_width, zoom_level, offset_x, offset_y);

        // Render the shadow under the character
        int shadow_radius = (character_tile_width / 3) * zoom_level;
        int shadow_x = (screen_width / 2 - (character_tile_width * zoom_level) / 2);
        int shadow_y = screen_height / 2 + (character_tile_height / 2) * zoom_level - (9 * zoom_level);
        render_shadow(renderer, shadow_x, shadow_y, shadow_radius);

        // Adjust the character's rendering position
        SDL_FRect character_rect = {
            (float)(screen_width / 2 - (character_tile_width * zoom_level)),
            (float)(screen_height / 2 - (character_tile_height * zoom_level) + (8 * zoom_level)),
            (float)(character_tile_width * zoom_level),
            (float)(character_tile_height * zoom_level)};

        // Calculate the source rectangle for the character animation
        int src_x = character.anim_ctrl.frame * character_tile_width;
        int src_y = character.anim_ctrl.direction * character_tile_height;
        SDL_FRect character_src_rect = {src_x, src_y, character_tile_width, character_tile_height};

        // Render the character from the tileset
        SDL_RenderTexture(renderer, character_tileset, &character_src_rect, &character_rect);

        render_button(renderer, BUTTON_ZOOM_IN);
        render_button(renderer, BUTTON_ZOOM_OUT);
        // render_button(renderer, BUTTON_SAVE_GAME); // Commented out

        render_ui(renderer, item_tilemap, &inventory_selection, screen_width, screen_height);
        render_crops(renderer, crop_texture, &crop_manager, tilemap_width, zoom_level, offset_x, offset_y);

        SDL_Color white = {255, 255, 255, 255};
        char fps_text[10];
        snprintf(fps_text, sizeof(fps_text), "FPS: %d", fps);
        render_text(renderer, fps_text, white, screen_width - 110, 10, 100, 30); // Move FPS text to top right

        if (show_debug_info) // Render debug info if flag is set
        {
            char debug_text[256];
            snprintf(debug_text, sizeof(debug_text),
                     "X: %d "
                     "Y: %d "
                     "Tile X: %d "
                     "Tile Y: %d "
                     "Look X: %d "
                     "Look Y: %d "
                     "Look Tile X: %d "
                     "Look Tile Y: %d "
                     "Direction: %d "
                     "Is Walking: %d "
                     "Equipped Tool: %d "
                     "Selected Main Item: %d "
                     "Selected Aux Item: %d "
                     "Selected Aux Inventory: %d",
                     character.x, character.y, character.tile_x, character.tile_y,
                     character.look_x, character.look_y, character.look_tile_x, character.look_tile_y,
                     character.anim_ctrl.direction, character.anim_ctrl.is_walking,
                     character.equipped_tool, inventory_selection.selected_main_item,
                     inventory_selection.selected_aux_item, inventory_selection.selected_aux_inventory);
            render_text(renderer, debug_text, white, screen_width - 1410, 50, 1400, 30); // Render debug info below FPS
        }

        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 16)
        {
            SDL_Delay(16 - frame_time);
        }
    }

    destroy_grid(grid);
    SDL_DestroyTexture(character_tileset);
    SDL_DestroyTexture(tilemap);
    SDL_DestroyTexture(item_tilemap);
    SDL_DestroyTexture(crop_texture);
}
