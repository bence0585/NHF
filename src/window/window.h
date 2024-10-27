#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Initialization functions
int initialize_SDL();
SDL_Window *create_window(const char *title, int width, int height);
SDL_Renderer *create_renderer(SDL_Window *window);

// Grid functions
void read_grid_state(const char *filename);
void render_grid(SDL_Renderer *renderer, SDL_Texture **textures, double zoom_level, int offset_x, int offset_y);
// Texture functions
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path);

// Event loop
void event_loop(SDL_Renderer *renderer);

// UI functions
void render_ui(SDL_Renderer *renderer);
bool is_zoom_in_button_clicked(int x, int y);
bool is_zoom_out_button_clicked(int x, int y);

// Cleanup function
void cleanup(SDL_Renderer *renderer, SDL_Window *window);

#endif // WINDOW_H