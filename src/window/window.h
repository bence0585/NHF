#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#define INVENTORY_SIZE 9
static const int TILE_SIZE = 16;

/*
 * Eszköz típusok
 */
typedef enum
{
    TOOL_HOE,          // Kapa
    TOOL_WATERING_CAN, // Öntözőkanna
    TOOL_SICKLE,       // Sarló
} ToolType;

typedef struct
{
    int width;
    int height;
    int **physical_layer;
    int **optical_layer;
    bool **collision_layer;
} Grid;

typedef struct
{
    int width;
    int height;
    int **background_layer;
    int **foreground_layer;
} ForegroundGrid;

/*
 * Animációk irányai
 */
typedef enum
{
    DIRECTION_DOWN,  // Lefelé
    DIRECTION_UP,    // Felfelé
    DIRECTION_RIGHT, // Jobbra
    DIRECTION_LEFT   // Balra
} Direction;

/*
 * Animáció vezérlő
 * frame: aktuális képkocka
 * max_frames: maximális képkocka
 * frame_delay: képkocka késleltetés
 * frame_delay_counter: aktuális képkocka késleltetés számláló
 * frame_direction: képkocka léptetésének irány
 * direction: karakter iránya
 * is_walking: mozog-e
 */
typedef struct
{
    int frame;
    int max_frames;
    int frame_delay;
    int frame_delay_counter;
    int frame_direction;
    Direction direction;
    bool is_walking;
} AnimationController;
/*
    * Karakter
    * x: x pozíció
    * y: y pozíció
    * tile_x: csempe x pozíció
    * tile_y: csempe y pozíció
    * look_x: nézés x pozíció
    * look_y: nézés y pozíció
    * look_tile_x: nézés csempe x pozíció
    * look_tile_y: nézés csempe y pozíció
    * equipped_tool: felszerelt eszköz
    * anim_ctrl: animáció vezérlő

*/
typedef struct
{
    int x;
    int y;
    int tile_x;
    int tile_y;
    int look_x;
    int look_y;
    int look_tile_x;
    int look_tile_y;
    ToolType equipped_tool;
    AnimationController anim_ctrl;
} Character;

/**
 * Mag típusok
 */
typedef enum
{
    SEED_PARSNIP,     // Parsnip
    SEED_CAULIFLOWER, // Karfiol
    SEED_COFFEE,      // Kávé
    SEED_GREEN_BEAN,  // Zöldbab
    SEED_HOPS,        // Komló
    SEED_POTATO,      // Burgonya
    SEED_STRAWBERRY,  // Eper
    SEED_MELON,       // Dinnye
    SEED_STARFRUIT,   // Csillaggyümölcs
} SeedType;

/**
 * Mag árak
 */
typedef enum
{
    SEED_PRICE_PARSNIP = 20,     // Parsnip ára
    SEED_PRICE_CAULIFLOWER = 80, // Karfiol ára
    SEED_PRICE_COFFEE = 15,      // Kávé ára
    SEED_PRICE_GREEN_BEAN = 60,  // Zöldbab ára
    SEED_PRICE_HOPS = 30,        // Komló ára
    SEED_PRICE_POTATO = 50,      // Burgonya ára
    SEED_PRICE_STRAWBERRY = 100, // Eper ára
    SEED_PRICE_MELON = 80,       // Dinnye ára
    SEED_PRICE_STARFRUIT = 200,  // Csillaggyümölcs ára
} SeedPrice;

/**
 * Termény árak
 */
typedef enum
{
    CROP_PRICE_PARSNIP = 35,      // Parsnip ára
    CROP_PRICE_CAULIFLOWER = 175, // Karfiol ára
    CROP_PRICE_COFFEE = 20,       // Kávé ára
    CROP_PRICE_GREEN_BEAN = 40,   // Zöldbab ára
    CROP_PRICE_HOPS = 25,         // Komló ára
    CROP_PRICE_POTATO = 80,       // Burgonya ára
    CROP_PRICE_STRAWBERRY = 120,  // Eper ára
    CROP_PRICE_MELON = 250,       // Dinnye ára
    CROP_PRICE_STARFRUIT = 750,   // Csillaggyümölcs ára
} CropPrice;

/*
 * Inventárválasztó
 * selected_main_item: kiválasztott fő elem
 * selected_aux_item: kiválasztott segéd elem
 * selected_aux_inventory: kiválasztott segéd tároló (0: nincs, 1: magtár, 2: betakarított terménytár)
 * seed_counts: magok száma
 * harvest_counts: betakarított termények száma
 * seed_types: mag típusok
 * total_seeds: összes mag
 * money: pénz
 */
typedef struct
{
    int selected_main_item;
    int selected_aux_item;
    int selected_aux_inventory;
    int seed_counts[INVENTORY_SIZE];
    int harvest_counts[INVENTORY_SIZE];
    SeedType seed_types[INVENTORY_SIZE];
    int total_seeds;
    int money;
} InventorySelection;

// Inicializálás

int initialize_SDL();
int initialize_SDLTTF();
SDL_Window *create_window(const char *title, int width, int height);
SDL_Renderer *create_renderer(SDL_Window *window);

// Text rendering functions
int initialize_font(const char *font_path, int font_size);
void render_text(SDL_Renderer *renderer, const char *text, SDL_Color color, float x, float y, float w, float h);
void cleanup_font();

// Rács függvények
Grid *create_grid(int width, int height);
void destroy_grid(Grid *grid);
void read_grid_state(const char *filename, Grid *grid);
void read_collision_data(const char *filename, Grid *grid);
void toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y);
void save_grid_state(const char *filename, Grid *grid);
void save_foreground_grid_state(const char *filename, ForegroundGrid *grid);
void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y);
void render_visible_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, Grid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y, int screen_width, int screen_height);
// Korrdinátaváltó függvény
void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y);
// Rács-négyzet kiemelése
void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y);
void highlight_look_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y);
// Textúra függvények
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path);

// Event loop
void event_loop(SDL_Renderer *renderer, Grid *background_grid, ForegroundGrid *foreground_grid);

// UI függvények
void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height, int inventory_y, int item_offset);
void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height);
bool is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot);

void save_game_state(const char *filename, int character_x, int character_y, InventorySelection *inventory_selection);
void load_game_state(const char *filename, int *character_x, int *character_y, InventorySelection *inventory_selection);

/**
 * Gomb típusok
 */
typedef enum
{
    BUTTON_ZOOM_IN,   // Nagyítás gomb
    BUTTON_ZOOM_OUT,  // Kicsinyítés gomb
    BUTTON_SAVE_GAME, // Játék mentése gomb
} ButtonType;

void render_button(SDL_Renderer *renderer, ButtonType button);
bool is_button_clicked(ButtonType button, int x, int y);

/**
 * Animáció frissítése
 * @param anim_ctrl animációvezérlő mutatója
 */
void update_animation(AnimationController *anim_ctrl);

/**
 * Csempe típusok
 */
typedef enum
{
    TILE_EMPTY,               // Üres csempe
    TILE_HOE = 229,           // Kapált föld
    TILE_WATERED,             // Öntözött föld
    TILE_HOE_PLANTED,         // Kapált és ültetett föld
    TILE_HOE_PLANTED_WATERED, // Kapált, ültetett és öntözött föld

} TileType;

/**
 * Eszköz műveletek
 */
typedef enum
{
    TOOL_ACTION_HOE,   // Kapálás
    TOOL_ACTION_WATER, // Öntözés
} ToolAction;

void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type);
void update_tile_texture(Grid *grid, int grid_x, int grid_y);
TileType get_tile_type(Grid *grid, int grid_x, int grid_y);

/**
 * Növény típusok
 */
typedef enum
{
    CROP_PARSNIP,     // Paszternák
    CROP_CAULIFLOWER, // Karfiol
    CROP_COFFEE,      // Kávé
    CROP_GREEN_BEAN,  // Zöldbab
    CROP_HOPS,        // Komló
    CROP_POTATO,      // Burgonya
    CROP_STRAWBERRY,  // Eper
    CROP_MELON,       // Dinnye
    CROP_STARFRUIT,   // Csillaggyümölcs
    // Add more crop types here
} CropType;

/**
 * Növény információk
 * type: növény típusa
 * name: növény neve
 * growth_stages: növekedési fázisok száma
 * texture_start: textúra kezdő pozíciója
 */
typedef struct
{
    CropType type;
    const char *name;
    int growth_stages;
    int texture_start;
} CropInfo;

static const CropInfo crop_info[] = {
    {CROP_PARSNIP, "Parsnip", 6, 0x10},
    {CROP_CAULIFLOWER, "Cauliflower", 7, 0x30},
    {CROP_COFFEE, "Coffee", 7, 0x90},
    {CROP_GREEN_BEAN, "Green Bean", 7, 0x18},
    {CROP_HOPS, "Hops", 7, 0x58},
    {CROP_POTATO, "Potato", 7, 0x38},
    {CROP_STRAWBERRY, "Strawberry", 7, 0x50},
    {CROP_MELON, "Melon", 7, 0x78},
    {CROP_STARFRUIT, "Starfruit", 7, 0x70},
    // Add more crop info here
};

/**
 * Növény fázisok
 */
typedef enum
{
    CROP_PHASE_1,  // 1. fázis
    CROP_PHASE_2,  // 2. fázis
    CROP_PHASE_3,  // 3. fázis
    CROP_PHASE_4,  // 4. fázis
    CROP_PHASE_5,  // 5. fázis
    CROP_PHASE_6,  // 6. fázis
    CROP_PHASE_7,  // 7. fázis
    CROP_PHASE_MAX // Maximális növekedési fázis
} CropPhase;

/**
 * Növény
 * x: x koordináta
 * y: y koordináta
 * type: növény típusa
 * growth_stage: növekedési fázis
 * growth_time: növekedési idő
 * current_time: aktuális növekedési idő
 */
typedef struct
{
    int x;
    int y;
    CropType type;
    CropPhase growth_stage;
    int growth_time;
    int current_time;
} Crop;

/**
 * Növénykezelő
 * crops: növények tömbje
 * crop_count: növények száma
 */
typedef struct
{
    Crop *crops;
    int crop_count;
} CropManager;

void initialize_crop_manager(CropManager *crop_manager);
void add_crop(CropManager *crop_manager, int x, int y, CropType type, int growth_time);
void update_crops(CropManager *crop_manager, int ticks, Grid *grid);
void render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y);
void save_crop_state(const char *filename, CropManager *crop_manager);
void load_crop_state(const char *filename, CropManager *crop_manager);

// Tick system
void game_tick(CropManager *crop_manager, int ticks, Grid *grid);

void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection);
void handle_crop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection);
void handle_harvest_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager, InventorySelection *inventory_selection);

// Takarítás
void cleanup(SDL_Renderer *renderer, SDL_Window *window);

void determine_grid_size(const char *filename, int *width, int *height);
void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type);
void update_tile_texture(Grid *grid, int grid_x, int grid_y);

ForegroundGrid *create_foreground_grid(int width, int height);
void destroy_foreground_grid(ForegroundGrid *grid);
void render_foreground_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, ForegroundGrid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y);
void read_foreground_grid_state(const char *filename, ForegroundGrid *grid); // Add this declaration
void clear_foreground_grid(ForegroundGrid *grid);

// Movement
void handle_movement(const bool *state, int *character_x, int *character_y, AnimationController *anim_ctrl, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height);

void initialize_character(Character *character, int start_x, int start_y);
void update_character_tile(Character *character, int tile_size);
void handle_character_movement(const bool *state, Character *character, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height);
void calculate_look_coordinates(Character *character, int tile_size);
void update_look_tile(Character *character, int tile_size);

void handle_aux_selection(int *selected_aux_item, int inventory_size, int offset);

static const int background_collision_tiles[] = {
    0x89,
    // Add more background collision tiles here
};

static const int foreground_collision_tiles[] = {
    0xE8, // Replace 0xXX with actual hex code
    0xE9, // Add more foreground collision tiles here
    0xD8, // Replace 0xXX with actual hex code
    0xD9, // Add more foreground collision tiles here

    // Add more foreground collision tiles here
};

void update_collision_data(const char *filename, Grid *grid, ForegroundGrid *fg_grid);

void handle_shop_action(Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, InventorySelection *inventory_selection);

#endif // WINDOW_H