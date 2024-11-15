#include "window.h"
/*
 * Kirajzolja a UI-t
 * @param renderer: a kirajzolást végző renderer
 */
void render_ui(SDL_Renderer *renderer)
{
    SDL_FRect zoom_in_button = {10.0f, 10.0f, 50.0f, 50.0f};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    SDL_RenderFillRect(renderer, &zoom_in_button);

    SDL_FRect zoom_out_button = {70.0f, 10.0f, 50.0f, 50.0f};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &zoom_out_button);

    SDL_FRect save_game_button = {130.0f, 10.0f, 50.0f, 50.0f};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color
    SDL_RenderFillRect(renderer, &save_game_button);
}

void render_button(SDL_Renderer *renderer, ButtonType button)
{
    SDL_FRect button_rect;
    switch (button)
    {
    case BUTTON_ZOOM_IN:
        button_rect = (SDL_FRect){10.0f, 10.0f, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
        break;
    case BUTTON_ZOOM_OUT:
        button_rect = (SDL_FRect){70.0f, 10.0f, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        break;
    case BUTTON_SAVE_GAME:
        button_rect = (SDL_FRect){130.0f, 10.0f, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color
        break;
        // Add more cases for additional buttons
    }
    SDL_RenderFillRect(renderer, &button_rect);
}

bool is_button_clicked(ButtonType button, int x, int y)
{
    switch (button)
    {
    case BUTTON_ZOOM_IN:
        return (x >= 10 && x <= 60 && y >= 10 && y <= 60);
    case BUTTON_ZOOM_OUT:
        return (x >= 70 && x <= 120 && y >= 10 && y <= 60);
    case BUTTON_SAVE_GAME:
        return (x >= 130 && x <= 180 && y >= 10 && y <= 60);
        // Add more cases for additional buttons
    }
    return false;
}