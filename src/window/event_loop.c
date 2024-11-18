#include <stdio.h>
#include "window.h"
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
        anim_ctrl->frame = 6 + (anim_ctrl->frame + 1) % 6; // Walking frames 6-11
        return;
    }

    // Idle animation: 0-1-2-3-4-5-4-3-2-1-0
    if (anim_ctrl->frame < 5 && anim_ctrl->frame_direction == 1)
    {
        anim_ctrl->frame++;
    }
    else if (anim_ctrl->frame > 0 && anim_ctrl->frame_direction == -1)
    {
        anim_ctrl->frame--;
    }
    else
    {
        anim_ctrl->frame_direction *= -1;
        anim_ctrl->frame += anim_ctrl->frame_direction;
    }
}

void game_tick(CropManager *crop_manager, int ticks)
{
    update_crops(crop_manager, ticks);
}

/*
 * A fő eseménykezelő függvény
 * @param renderer: a kirajzolást végző renderer
 */
void event_loop(SDL_Renderer *renderer)
{
    SDL_Event event;
    int quit = 0;
    double zoom_level = 1;
    const int tilemap_width = 16;
    const int tilemap_height = tilemap_width;
    const int character_tile_width = tilemap_width;
    const int character_tile_height = tilemap_width * 2;
    int tile_size = tilemap_width * 2;
    int character_x = 0, character_y = 0; // Start at top-left tile

    load_game_state("../src/save_state.txt", &character_x, &character_y);

    SDL_Texture *tilemap = load_texture(renderer, "../src/img/tilemap.png");
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);

    SDL_Texture *character_tileset = load_texture(renderer, "../src/img/playerTilemap.png");
    SDL_Texture *item_tilemap = load_texture(renderer, "../src/img/itemTilemap.png");
    SDL_Texture *crop_texture = load_texture(renderer, "../src/img/cropTilemap.png"); // Crop texture
    int selected_item = 0;

    int grid_width, grid_height;
    determine_grid_size("../src/grid_state.txt", &grid_width, &grid_height);
    Grid *grid = create_grid(grid_width, grid_height);
    read_grid_state("../src/grid_state.txt", grid);
    if (grid == NULL)
    {
        SDL_Log("Failed to read grid state.");
        return;
    }

    CropManager crop_manager;
    initialize_crop_manager(&crop_manager);

    AnimationController anim_ctrl = {0, 6, 10, 0, 1, DIRECTION_DOWN, false}; // Initialize frame_direction to 1

    Uint32 start_time = SDL_GetTicks();
    int frame_count = 0;
    int fps = 0;

    static const int movement_speed = 1;

    int screen_width, screen_height;
    SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);

    ToolType selected_tool = TOOL_HOE; // Default tool

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

        int grid_x, grid_y;
        convert_to_grid_coordinates(character_x, character_y + character_tile_height / 2, tilemap_width, &grid_x, &grid_y);

        // Calculate the grid coordinates the player is looking at
        int look_x = grid_x;
        int look_y = grid_y;
        switch (anim_ctrl.direction)
        {
        case DIRECTION_UP:
            look_y -= 1;
            break;
        case DIRECTION_DOWN:
            look_y += 1;
            break;
        case DIRECTION_LEFT:
            look_x -= 1;
            break;
        case DIRECTION_RIGHT:
            look_x += 1;
            break;
        }

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
                }
                else if (event.key.key >= SDLK_1 && event.key.key <= SDLK_9)
                {
                    selected_item = event.key.key - SDLK_1;
                    if (selected_item >= INVENTORY_SIZE)
                    {
                        selected_item = INVENTORY_SIZE - 1;
                    }
                }
                else if (event.key.key == SDLK_C)
                {
                    handle_tool_action(selected_tool, grid, look_x, look_y, &crop_manager);
                }
                break;

            case SDL_EVENT_MOUSE_WHEEL:
                if (event.wheel.y > 0) // Scroll up
                {
                    selected_item = (selected_item - 1 + INVENTORY_SIZE) % INVENTORY_SIZE;
                }
                else if (event.wheel.y < 0) // Scroll down
                {
                    selected_item = (selected_item + 1) % INVENTORY_SIZE;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (is_button_clicked(BUTTON_ZOOM_IN, x, y))
                    {
                        zoom_level += 1;
                    }
                    else if (is_button_clicked(BUTTON_ZOOM_OUT, x, y))
                    {
                        zoom_level -= 1;
                    }
                    else if (is_button_clicked(BUTTON_SAVE_GAME, x, y))
                    {
                        save_game_state("../src/save_state.txt", character_x, character_y);
                    }
                    else
                    {
                        int slot;
                        if (is_inventory_slot_clicked(x, y, screen_width, screen_height, &slot))
                        {
                            selected_item = slot;
                        }
                        else
                        {
                            handle_tool_action(selected_tool, grid, look_x, look_y, &crop_manager);
                        }
                    }
                    // Add more button checks here
                }
            }
        }

        anim_ctrl.is_walking = false;
        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
        {
            if (character_y > 0)
            {
                character_y -= movement_speed;
                anim_ctrl.direction = DIRECTION_UP;
                anim_ctrl.is_walking = true;
            }
        }
        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
        {
            if (character_y < (grid->height * tile_size - character_tile_height))
            {
                character_y += movement_speed;
                anim_ctrl.direction = DIRECTION_DOWN;
                anim_ctrl.is_walking = true;
            }
        }
        if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
        {
            if (character_x > 0)
            {
                character_x -= movement_speed;
                anim_ctrl.direction = DIRECTION_LEFT;
                anim_ctrl.is_walking = true;
            }
        }
        if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
        {
            if (character_x < (grid->width * tile_size - character_tile_width))
            {
                character_x += movement_speed;
                anim_ctrl.direction = DIRECTION_RIGHT;
                anim_ctrl.is_walking = true;
            }
        }

        update_animation(&anim_ctrl);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character_x * zoom_level - (tilemap_width * zoom_level) / 2;
        int offset_y = screen_height / 2 - character_y * zoom_level - (tilemap_height * zoom_level) / 2;

        render_grid(renderer, tilemap, grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);
        render_visible_grid(renderer, tilemap, grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y, screen_width, screen_height);

        highlight_grid_square(renderer, grid_x, grid_y, tilemap_width, zoom_level, offset_x, offset_y);
        highlight_look_square(renderer, look_x, look_y, tilemap_width, zoom_level, offset_x, offset_y);

        // Adjust the character's rendering position
        SDL_FRect character_rect = {
            (float)(screen_width / 2 - (character_tile_width * zoom_level)),
            (float)(screen_height / 2 - (character_tile_height * zoom_level) + (8 * zoom_level)),
            (float)(character_tile_width * zoom_level),
            (float)(character_tile_height * zoom_level)};

        // Calculate the source rectangle for the character animation
        int src_x = anim_ctrl.frame * character_tile_width;
        int src_y = anim_ctrl.direction * character_tile_height;
        SDL_FRect character_src_rect = {src_x, src_y, character_tile_width, character_tile_height};

        // Render the character from the tileset
        SDL_RenderTexture(renderer, character_tileset, &character_src_rect, &character_rect);

        render_button(renderer, BUTTON_ZOOM_IN);
        render_button(renderer, BUTTON_ZOOM_OUT);
        // render_button(renderer, BUTTON_SAVE_GAME); // Commented out

        render_ui(renderer, item_tilemap, selected_item, screen_width, screen_height);
        render_crops(renderer, crop_texture, &crop_manager, tilemap_width, zoom_level, offset_x, offset_y);

        SDL_Color white = {255, 255, 255, 255};
        char fps_text[10];
        snprintf(fps_text, sizeof(fps_text), "FPS: %d", fps);
        render_text(renderer, fps_text, white, 10, 10, 100, 30);

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