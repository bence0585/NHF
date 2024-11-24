#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#define INVENTORY_SIZE 9
static const int TILE_SIZE = 16;

typedef enum
{
    TOOL_HOE,
    TOOL_WATERING_CAN,
    TOOL_SICKLE,
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

// Animációk típusai
typedef enum
{
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_LEFT
} Direction;

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

typedef struct
{
    int x;
    int y;
    int tile_x;
    int tile_y;
    int look_x;
    int look_y; // Add look_x and look_y fields
    int look_tile_x;
    int look_tile_y; // Add look_tile_x and look_tile_y fields
    ToolType equipped_tool;
    AnimationController anim_ctrl;
} Character;

typedef enum
{
    SEED_PARSNIP,
    SEED_CAULIFLOWER,
    SEED_COFFEE,
    SEED_GREEN_BEAN,
    SEED_HOPS,
    SEED_POTATO,
    SEED_STRAWBERRY,
    SEED_MELON,
    SEED_STARFRUIT,
    // Add more seed types here
} SeedType;

typedef enum
{
    SEED_PRICE_PARSNIP = 20,
    SEED_PRICE_CAULIFLOWER = 80,
    SEED_PRICE_COFFEE = 15,
    SEED_PRICE_GREEN_BEAN = 60,
    SEED_PRICE_HOPS = 30,
    SEED_PRICE_POTATO = 50,
    SEED_PRICE_STRAWBERRY = 100,
    SEED_PRICE_MELON = 80,
    SEED_PRICE_STARFRUIT = 200,
    // Add more seed prices here
} SeedPrice;

typedef enum
{
    CROP_PRICE_PARSNIP = 35,
    CROP_PRICE_CAULIFLOWER = 175,
    CROP_PRICE_COFFEE = 20,
    CROP_PRICE_GREEN_BEAN = 40,
    CROP_PRICE_HOPS = 25,
    CROP_PRICE_POTATO = 80,
    CROP_PRICE_STRAWBERRY = 120,
    CROP_PRICE_MELON = 250,
    CROP_PRICE_STARFRUIT = 750,
    // Add more crop prices here
} CropPrice;

typedef struct
{
    int selected_main_item;
    int selected_aux_item;
    int selected_aux_inventory;          // 0 for none, 1 for seed pouch, 2 for harvest bag
    int seed_counts[INVENTORY_SIZE];     // Counts for seeds
    int harvest_counts[INVENTORY_SIZE];  // Counts for harvests
    SeedType seed_types[INVENTORY_SIZE]; // Types of seeds
    int total_seeds;                     // Add total_seeds field
    int money;                           // Add money field
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

typedef enum
{
    BUTTON_ZOOM_IN,
    BUTTON_ZOOM_OUT,
    BUTTON_SAVE_GAME,
} ButtonType;

void render_button(SDL_Renderer *renderer, ButtonType button);
bool is_button_clicked(ButtonType button, int x, int y);

// Animation
void update_animation(AnimationController *anim_ctrl);

typedef enum
{
    TILE_EMPTY,
    TILE_HOE = 229, // Hoed ground
    TILE_WATERED,
    TILE_HOE_PLANTED,         // Hoed and planted ground
    TILE_HOE_PLANTED_WATERED, // Hoed, planted, and watered ground
    TILE_CROP_1,
    TILE_CROP_2,
    TILE_CROP_3,
    // Add more tile types here
} TileType;

typedef enum
{
    TOOL_ACTION_HOE,
    TOOL_ACTION_WATER,
} ToolAction;

void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type);
void update_tile_texture(Grid *grid, int grid_x, int grid_y);
TileType get_tile_type(Grid *grid, int grid_x, int grid_y);

typedef enum
{
    CROP_PARSNIP,
    CROP_CAULIFLOWER,
    CROP_COFFEE,
    CROP_GREEN_BEAN,
    CROP_HOPS,
    CROP_POTATO,
    CROP_STRAWBERRY,
    CROP_MELON,
    CROP_STARFRUIT,
    // Add more crop types here
} CropType;

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

typedef enum
{
    CROP_PHASE_1,
    CROP_PHASE_2,
    CROP_PHASE_3,
    CROP_PHASE_4,
    CROP_PHASE_5,
    CROP_PHASE_6,
    CROP_PHASE_7,
    CROP_PHASE_MAX // Maximum growth phase
} CropPhase;

typedef struct
{
    int x;
    int y;
    CropType type;
    CropPhase growth_stage;
    int growth_time;  // Time required to grow to the next stage
    int current_time; // Current time spent growing
} Crop;

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