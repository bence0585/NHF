#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

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
bool is_zoom_in_button_clicked(int x, int y);
bool is_zoom_out_button_clicked(int x, int y);

// Takarítás
void cleanup(SDL_Renderer *renderer, SDL_Window *window);

#endif // WINDOW_H