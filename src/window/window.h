#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>
static const int GRID_WIDTH = 32;
static const int GRID_HEIGHT = 32;
static const int TILE_SIZE = 16;

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
    Direction direction;
    bool is_walking;
} AnimationController;

// Inicializálás

int initialize_SDL();
SDL_Window *create_window(const char *title, int width, int height);
SDL_Renderer *create_renderer(SDL_Window *window);

// Rács függvények
void read_grid_state(const char *filename);
void render_grid(SDL_Renderer *renderer, SDL_Texture *tilemap, int tilemap_width, int tilemap_height, double zoom_level, int offset_x, int offset_y);
// Korrdinátaváltó függvény
void convert_to_grid_coordinates(int character_x, int character_y, int tile_size, int *grid_x, int *grid_y);
// Rács-négyzet kiemelése
void highlight_grid_square(SDL_Renderer *renderer, int grid_x, int grid_y, int tile_size, double zoom_level, int offset_x, int offset_y);
// Textúra függvények
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path);

// Event loop
void event_loop(SDL_Renderer *renderer);

// UI függvények
void render_ui(SDL_Renderer *renderer);

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

// Takarítás
void cleanup(SDL_Renderer *renderer, SDL_Window *window);

#endif // WINDOW_H