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

void save_game_state(const char *filename, int character_x, int character_y)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Error opening save file: %s", strerror(errno));
        return;
    }
    fprintf(file, "%d %d\n", character_x, character_y);
    fclose(file);
}

void load_game_state(const char *filename, int *character_x, int *character_y)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Save file not found, creating a new one.");
        *character_x = 0;
        *character_y = 0;
        save_game_state(filename, *character_x, *character_y);
        return;
    }
    fscanf(file, "%d %d", character_x, character_y);
    fclose(file);
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

    // SDL_Texture *character_texture = load_texture(renderer, "../src/img/character.png");

    SDL_Texture *tilemap = load_texture(renderer, "../src/img/tilemap.png");
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);

    SDL_Texture *character_tileset = load_texture(renderer, "../src/img/playerTilemap.png");

    read_grid_state("../src/grid_state.txt");

    AnimationController anim_ctrl = {0, 6, 10, 0, 1, DIRECTION_DOWN, false}; // Initialize frame_direction to 1

    /*A fő eseménykezelő ciklus*/
    static const int movement_speed = 1;
    while (!quit)
    {
        SDL_PumpEvents();
        const bool *state = SDL_GetKeyboardState(NULL);

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
            if (character_y < (GRID_HEIGHT * TILE_SIZE - character_tile_height))
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
            if (character_x < (GRID_WIDTH * TILE_SIZE - character_tile_width))
            {
                character_x += movement_speed;
                anim_ctrl.direction = DIRECTION_RIGHT;
                anim_ctrl.is_walking = true;
            }
        }

        update_animation(&anim_ctrl);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0x0, 0x0);
        SDL_RenderClear(renderer);

        int screen_width, screen_height;
        SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);
        int offset_x = screen_width / 2 - character_x * zoom_level - (tilemap_width * zoom_level) / 2;
        int offset_y = screen_height / 2 - character_y * zoom_level - (tilemap_height * zoom_level) / 2;

        render_grid(renderer, tilemap, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);
        render_visible_grid(renderer, tilemap, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y, screen_width, screen_height);

        int grid_x, grid_y;
        convert_to_grid_coordinates(character_x, character_y + character_tile_height / 2, tilemap_width, &grid_x, &grid_y);

        highlight_grid_square(renderer, grid_x, grid_y, tilemap_width, zoom_level, offset_x, offset_y);

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

        render_ui(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(character_tileset);
    SDL_DestroyTexture(tilemap);
}