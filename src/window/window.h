#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#define INVENTORY_SIZE 9
#define TILE_SIZE 16

typedef enum
{
    TOOL_HOE,
    TOOL_WATERING_CAN,
    TOOL_SICKLE,
    // Add more tools here
} ToolType;

typedef struct
{
    int width;
    int height;
    int **physical_layer;
    int **optical_layer;
    bool **collision_layer; // Add collision layer
} Grid;

typedef struct
{
    int width;
    int height;
    int **background_layer;
    int **foreground_layer;
} ForegroundGrid;

// Animation types
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
    int frame_direction; // New property for idle animation direction
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
void read_collision_data(const char *filename, Grid *grid);                           // Add this declaration
void toggle_collision_data(const char *filename, Grid *grid, int grid_x, int grid_y); // Add this declaration
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
void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, int selected_item, int screen_width, int screen_height);
void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, int selected_item, int screen_width, int screen_height);
bool is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot);

void save_game_state(const char *filename, int character_x, int character_y);
void load_game_state(const char *filename, int *character_x, int *character_y);

typedef enum
{
    BUTTON_ZOOM_IN,
    BUTTON_ZOOM_OUT,
    BUTTON_SAVE_GAME,
    // Add more buttons here
} ButtonType;

void render_button(SDL_Renderer *renderer, ButtonType button);
bool is_button_clicked(ButtonType button, int x, int y);

// Animation
void update_animation(AnimationController *anim_ctrl);

typedef enum
{
    TILE_EMPTY,
    TILE_HOE,
    TILE_WATERED,
    // Add more tile types here
} TileType;

typedef enum
{
    TOOL_ACTION_HOE,
    TOOL_ACTION_WATER,
    // Add more tool actions here
} ToolAction;

void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type);
void update_tile_texture(Grid *grid, int grid_x, int grid_y);
void on_tile_change(Grid *grid, int grid_x, int grid_y, TileType tile_type);

typedef enum
{
    CROP_SEED_1,
    CROP_SEED_2,
    CROP_SEED_3,
    CROP_PRODUCT_1,
    CROP_PRODUCT_2,
    CROP_PRODUCT_3,
    // Add more crop types here
} CropType;

typedef struct
{
    int x;
    int y;
    int growth_stage;
    int growth_time;  // Time required to grow to the next stage
    int current_time; // Current time spent growing
} Crop;

typedef struct
{
    Crop *crops;
    int crop_count;
} CropManager;

void initialize_crop_manager(CropManager *crop_manager);
void add_crop(CropManager *crop_manager, int x, int y, int growth_time);
void update_crops(CropManager *crop_manager, int ticks);
void render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y);

// Tick system
void game_tick(CropManager *crop_manager, int ticks);

void handle_tool_action(ToolType tool, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager);
void handle_crop_action(CropType crop, Grid *grid, ForegroundGrid *fg_grid, int grid_x, int grid_y, CropManager *crop_manager);

// Takarítás
void cleanup(SDL_Renderer *renderer, SDL_Window *window);

void determine_grid_size(const char *filename, int *width, int *height);
void set_tile_type(Grid *grid, int grid_x, int grid_y, TileType tile_type);
void update_tile_texture(Grid *grid, int grid_x, int grid_y);

ForegroundGrid *create_foreground_grid(int width, int height);
void destroy_foreground_grid(ForegroundGrid *grid);
void render_foreground_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, ForegroundGrid *grid, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y);
void read_foreground_grid_state(const char *filename, ForegroundGrid *grid); // Add this declaration

// Movement
void handle_movement(const bool *state, int *character_x, int *character_y, AnimationController *anim_ctrl, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height);

void initialize_character(Character *character, int start_x, int start_y);
void update_character_tile(Character *character, int tile_size);
void handle_character_movement(const bool *state, Character *character, Grid *grid, int movement_speed, int tile_size, int character_tile_width, int character_tile_height);
void calculate_look_coordinates(Character *character, int tile_size);
void update_look_tile(Character *character, int tile_size);

#endif // WINDOW_H