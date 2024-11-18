#include "window.h"

#define INVENTORY_SIZE 9
#define ITEM_SIZE 64       // Increased item size for scaled resolution
#define PADDING 4          // Padding between inventory tiles
#define BORDER_THICKNESS 4 // Thicker border

void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, int selected_item, int screen_width, int screen_height)
{
    int inventory_width = INVENTORY_SIZE * (ITEM_SIZE + PADDING) - PADDING;
    int inventory_x = (screen_width - inventory_width) / 2;
    int inventory_y = screen_height - ITEM_SIZE - 20; // 20 pixels from the bottom

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for borders

    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        SDL_FRect item_rect = {inventory_x + i * (ITEM_SIZE + PADDING), inventory_y, ITEM_SIZE, ITEM_SIZE};
        SDL_RenderRect(renderer, &item_rect);

        if (i == selected_item)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                                                                                                                // Red color for selected item border
            SDL_FRect highlight_rect = {item_rect.x - BORDER_THICKNESS, item_rect.y - BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS}; // Thicker border
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Reset to white color
        }

        // Render item from item_tilemap
        SDL_FRect src_rect = {i * 16, 0, 16, 16}; // Source size is 16x16
        SDL_RenderTexture(renderer, item_tilemap, &src_rect, &item_rect);
    }
}

void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, int selected_item, int screen_width, int screen_height)
{
    SDL_FRect zoom_in_button = {10.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);        // Green color
    SDL_RenderFillRect(renderer, &zoom_in_button);

    SDL_FRect zoom_out_button = {80.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);         // Red color
    SDL_RenderFillRect(renderer, &zoom_out_button);

    SDL_FRect save_game_button = {150.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);           // Blue color
    SDL_RenderFillRect(renderer, &save_game_button);

    render_inventory(renderer, item_tilemap, selected_item, screen_width, screen_height);
}

void render_button(SDL_Renderer *renderer, ButtonType button)
{
    SDL_FRect button_rect;
    switch (button)
    {
    case BUTTON_ZOOM_IN:
        button_rect = (SDL_FRect){10.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);      // Green color
        break;
    case BUTTON_ZOOM_OUT:
        button_rect = (SDL_FRect){80.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);      // Red color
        break;
    case BUTTON_SAVE_GAME:
        button_rect = (SDL_FRect){150.0f, 10.0f, 60.0f, 60.0f}; // Increased button size
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);       // Blue color
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
        return (x >= 10 && x <= 70 && y >= 10 && y <= 70); // Adjusted for increased button size
    case BUTTON_ZOOM_OUT:
        return (x >= 80 && x <= 140 && y >= 10 && y <= 70); // Adjusted for increased button size
    case BUTTON_SAVE_GAME:
        return (x >= 150 && x <= 210 && y >= 10 && y <= 70); // Adjusted for increased button size
        // Add more cases for additional buttons
    }
    return false;
}

bool is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot)
{
    int inventory_width = INVENTORY_SIZE * (ITEM_SIZE + PADDING) - PADDING;
    int inventory_x = (screen_width - inventory_width) / 2;
    int inventory_y = screen_height - ITEM_SIZE - 20; // 20 pixels from the bottom

    if (y >= inventory_y && y <= inventory_y + ITEM_SIZE)
    {
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            int slot_x = inventory_x + i * (ITEM_SIZE + PADDING);
            if (x >= slot_x && x <= slot_x + ITEM_SIZE)
            {
                *slot = i;
                return true;
            }
        }
    }
    return false;
}

// ...existing code...