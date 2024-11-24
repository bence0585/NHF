#include <stdio.h>
#include <math.h>
#include <SDL3/SDL_render.h> // SDL_RenderDrawPoint-hoz
#include "window.h"
/*
 * Visszaadja a megadott csempe típus nevét.
 * @param tile_type csempe típus
 * @return csempe típus neve
 */
const char *get_tile_type_name(TileType tile_type)
{
    switch (tile_type)
    {
    case TILE_EMPTY:
        return "TILE_EMPTY";
    case TILE_HOE:
        return "TILE_HOE";
    case TILE_WATERED:
        return "TILE_WATERED";
    default:
        return "UNKNOWN";
    }
}

/*
 * Árnyékot rajzol a megadott pozícióra adott sugárral.
 * @param renderer rajzoló mutatója
 * @param x x koordináta
 * @param y y koordináta
 * @param radius sugár
 * @return void
 * @note Az árnyék színe félig átlátszó fekete.
 */
void render_shadow(SDL_Renderer *renderer, int x, int y, int radius)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64); // Enyhén átlátszó fekete

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

/*
 * Tick rendszer frissítése a játékban.
 * @param crop_manager növénykezelő mutatója
 * @param ticks időegységek száma
 * @param grid rács mutatója
 * @return void
 * @note A játékot egy időegységgel előre lépteti minden másodpercben.
 */
void game_tick(CropManager *crop_manager, int ticks, Grid *grid)
{
    update_crops(crop_manager, ticks, grid);
}
/*
    * Billentyű lenyomás esemény kezelése.
    * @param event esemény mutatója
    * @param character karakter mutatója
    * @param inventory_selection inventárválasztó mutatója
    * @param grid rács mutatója
    * @param foreground_grid előtér rács mutatója
    * @param crop_manager növénykezelő mutatója
    * @param quit kilépési változó
    * @param show_debug_info debug információ megjelenítése
    * @return void
    * @note Az ESC gomb lenyomására kilép a játékból.
    * @note Az 1-9 számok lenyomására az inventárválasztóban választott tárgyakat állítja be.
    * @note A SHIFT + 1-9 számok lenyomására az aux tárgyakat állítja be.
    * @note A C gomb lenyomására a karakter által kiválasztott eszköz műveletét hajtja végre.
    * @note Az X gomb lenyomására a karakter által kiválasztott eszköz bolti műveletét hajtja végre.
    * @note Az L gomb lenyomására az ütközési adatokat módosítja.
    * @note Az F2 gomb lenyomására a debug információ megjelenítését kapcsolja.
    * @note A karakter által kiválasztott eszköz műveletei: kapálás, öntözés, betakarítás.
    * @note A karakter által kiválasztott eszköz bolti műveletei: vetőmag vásárlás, termény eladás.
    * @note Az ütközési adatok módosítása: az adott csempe ütközési állapotát módosítja.

*/
extern void handle_key_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info);

/*
 * Egér görgő esemény kezelése.
 * @param event esemény mutatója
 * @param character karakter mutatója
 * @param screen_width képernyő szélesség
 * @param screen_height képernyő magasság
 * @param zoom_level nagyítási szint
 * @return void
 * @note Az egér görgőjével a karakterláda tárgyait lehet váltani.
 * @note A SHIFT gomb lenyomásával a vetőmagokat és betakarított terményeket lehet váltani.
 */

extern void handle_mouse_wheel(SDL_Event *event, InventorySelection *inventory_selection);

/*
 * Egér gombok lenyomás esemény kezelése.
 * @param event esemény mutatója
 * @param character karakter mutatója
 * @param inventory_selection inventárválasztó mutatója
 * @param grid rács mutatója
 * @param foreground_grid előtér rács mutatója
 * @param crop_manager növénykezelő mutatója
 * @param screen_width képernyő szélesség
 * @param screen_height képernyő magasság
 * @param zoom_level nagyítási szint
 * @return void
 * @note Az egér bal gomb lenyomásával a karakterláda tárgyait lehet váltani.
 * @note Az egér bal gomb lenyomásával a nagyítási szintet lehet állítani.
 * @note Az egér bal gomb lenyomásával a játékállapotot lehet menteni.
 */
extern void handle_mouse_button_down(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height, int *zoom_level);

extern void render_debug_info(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int screen_width, int screen_height);

/*
 * Játék inicializálása.
 * @param renderer rajzoló mutatója
 * @param character karakter mutatója
 * @param inventory_selection inventárválasztó mutatója
 * @param grid rács mutatója
 * @param foreground_grid előtér rács mutatója
 * @param crop_manager növénykezelő mutatója
 * @param grid_width rács szélessége
 * @param grid_height rács magassága
 * @return void
 */
void initialize_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid **grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, int *grid_width, int *grid_height)
{
    initialize_character(character, 0, 0);                                                       // Inicializálja a karaktert a (0, 0) pozícióval (ez felül lesz írva a mentett játékállapotból)
    load_game_state("../src/save_state.txt", &character->x, &character->y, inventory_selection); // Betölti a játékállapotot
    update_character_tile(character, TILE_SIZE);                                                 // Frissíti a karakter csempéjét

    determine_grid_size("../src/grid_state.txt", grid_width, grid_height); // Meghatározza a rács méretét
    *grid = create_grid(*grid_width, *grid_height);                        // Létrehozza a rácsot
    read_grid_state("../src/grid_state.txt", *grid);                       // Beolvassa a rács állapotát
    read_collision_data("../src/collisions.txt", *grid);                   // Beolvassa az ütközési adatokat

    if (*grid == NULL)
    {
        SDL_Log("Failed to read grid state.");
        return;
    }

    initialize_crop_manager(crop_manager);                                           // Inicializálja a növénykezelőt
    load_crop_state("../src/crop_state.txt", crop_manager);                          // Betölti a növények állapotát
    read_foreground_grid_state("../src/foreground_grid_state.txt", foreground_grid); // Beolvassa az előtér rács állapotát
}

/*

*/
void render_game(SDL_Renderer *renderer, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, SDL_Texture *tilemap, SDL_Texture *character_tileset, SDL_Texture *item_tilemap, SDL_Texture *crop_texture, int screen_width, int screen_height, int zoom_level, int fps, bool show_debug_info)
{
    int tilemap_width = TILE_SIZE;
    int tilemap_height = TILE_SIZE;
    int character_tile_width = TILE_SIZE;
    int character_tile_height = TILE_SIZE * 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int offset_x = screen_width / 2 - character->x * zoom_level - (tilemap_width * zoom_level) / 2;
    int offset_y = screen_height / 2 - character->y * zoom_level - (tilemap_height * zoom_level) / 2;

    render_grid(renderer, tilemap, grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);
    render_foreground_grid(renderer, crop_texture, foreground_grid, tilemap_width, tilemap_height, zoom_level, offset_x, offset_y);
    render_crops(renderer, crop_texture, crop_manager, tilemap_width, zoom_level, offset_x, offset_y); // Render crops before character and UI

    highlight_grid_square(renderer, character->tile_x, character->tile_y, tilemap_width, zoom_level, offset_x, offset_y);
    highlight_look_square(renderer, character->look_tile_x, character->look_tile_y, tilemap_width, zoom_level, offset_x, offset_y);

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
    int src_x = character->anim_ctrl.frame * character_tile_width;
    int src_y = character->anim_ctrl.direction * character_tile_height;
    SDL_FRect character_src_rect = {src_x, src_y, character_tile_width, character_tile_height};

    // Render the character from the tileset
    SDL_RenderTexture(renderer, character_tileset, &character_src_rect, &character_rect);

    render_button(renderer, BUTTON_ZOOM_IN);
    render_button(renderer, BUTTON_ZOOM_OUT);
    // render_button(renderer, BUTTON_SAVE_GAME); // Commented out

    render_ui(renderer, item_tilemap, inventory_selection, screen_width, screen_height); // Render UI after character

    SDL_Color white = {255, 255, 255, 255};
    char fps_text[10];
    snprintf(fps_text, sizeof(fps_text), "FPS: %d", fps);
    render_text(renderer, fps_text, white, screen_width - 110, 10, 100, 30); // Move FPS text to top right

    if (show_debug_info) // Render debug info if flag is set
    {
        render_debug_info(renderer, character, inventory_selection, grid, foreground_grid, crop_manager, screen_width, screen_height);
    }

    SDL_RenderPresent(renderer);
}

void handle_events(SDL_Event *event, Character *character, InventorySelection *inventory_selection, Grid *grid, ForegroundGrid *foreground_grid, CropManager *crop_manager, bool *quit, bool *show_debug_info, int screen_width, int screen_height, int *zoom_level)
{
    // Események kezelése
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
        case SDL_EVENT_QUIT:
            SDL_Log("Kilepes: SDL3 event");
            *quit = true;
            break;
        case SDL_EVENT_KEY_DOWN:
            handle_key_down(event, character, inventory_selection, grid, foreground_grid, crop_manager, quit, show_debug_info);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            handle_mouse_wheel(event, inventory_selection);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            handle_mouse_button_down(event, character, inventory_selection, grid, foreground_grid, crop_manager, screen_width, screen_height, zoom_level);
            break;
        }
    }
}

extern void update_character_position(Character *character, Grid *grid, int grid_width, int grid_height, int tile_size, int character_tile_width, int character_tile_height);

void event_loop(SDL_Renderer *renderer, Grid *background_grid, ForegroundGrid *foreground_grid)
{
    SDL_Event event;
    bool quit = false;
    int zoom_level = 1;
    int screen_width, screen_height;
    SDL_GetCurrentRenderOutputSize(renderer, &screen_width, &screen_height);

    Character character;
    InventorySelection inventory_selection = {0, 0, 0}; // Inventárválasztó inicializálása
    Grid *grid;
    CropManager crop_manager;
    int grid_width, grid_height;

    initialize_game(renderer, &character, &inventory_selection, &grid, foreground_grid, &crop_manager, &grid_width, &grid_height);

    SDL_Texture *tilemap = load_texture(renderer, "../src/img/tilemap.png");
    SDL_SetTextureScaleMode(tilemap, SDL_SCALEMODE_NEAREST);
    SDL_Texture *character_tileset = load_texture(renderer, "../src/img/playerTilemap.png");
    SDL_SetTextureScaleMode(character_tileset, SDL_SCALEMODE_NEAREST);
    SDL_Texture *item_tilemap = load_texture(renderer, "../src/img/itemTilemap.png");
    SDL_SetTextureScaleMode(item_tilemap, SDL_SCALEMODE_NEAREST);
    SDL_Texture *crop_texture = load_texture(renderer, "../src/img/cropTilemap.png"); // Növény textúra
    SDL_SetTextureScaleMode(crop_texture, SDL_SCALEMODE_NEAREST);

    Uint32 start_time = SDL_GetTicks();
    int frame_count = 0;
    int fps = 0;
    bool show_debug_info = false; // Debug információ megjelenítésének kapcsolója

    /*
        * A játék fő ciklusa.
        * A játékot egy időegységgel előre lépteti minden másodpercben.
        * A karakter mozgatása, nézési irányának frissítése, animációk frissítése, játék állapotának frissítése, játék kirajzolása, képkocka időzítése.

    */
    while (!quit)
    {
        Uint32 frame_start = SDL_GetTicks();
        frame_count++;
        if (frame_start - start_time >= 1000)
        {
            fps = frame_count;
            frame_count = 0;
            start_time = frame_start;
            game_tick(&crop_manager, 1, grid); // A játékot egy időegységgel előre lépteti minden másodpercben
        }

        SDL_PumpEvents();
        const bool *state = SDL_GetKeyboardState(NULL);

        update_character_tile(&character, TILE_SIZE);
        int grid_x, grid_y;
        convert_to_grid_coordinates(character.x + TILE_SIZE / 2, character.y + TILE_SIZE, TILE_SIZE, &grid_x, &grid_y);

        handle_events(&event, &character, &inventory_selection, grid, foreground_grid, &crop_manager, &quit, &show_debug_info, screen_width, screen_height, &zoom_level);

        handle_character_movement(state, &character, grid, 2, TILE_SIZE, TILE_SIZE, TILE_SIZE * 2);
        calculate_look_coordinates(&character, TILE_SIZE); // Biztosítja, hogy a nézési koordináták frissüljenek

        update_character_position(&character, grid, grid_width, grid_height, TILE_SIZE, TILE_SIZE, TILE_SIZE * 2);

        update_animation(&character.anim_ctrl);

        render_game(renderer, &character, &inventory_selection, grid, foreground_grid, &crop_manager, tilemap, character_tileset, item_tilemap, crop_texture, screen_width, screen_height, zoom_level, fps, show_debug_info);

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
