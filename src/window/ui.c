#include "window.h"
#include <stdio.h>            // Include the correct header for snprintf
#include <SDL3_ttf/SDL_ttf.h> // Include SDL_ttf for text rendering

#define INVENTORY_SIZE 9
#define ITEM_SIZE 64       // Increased item size for scaled resolution
#define PADDING 4          // Padding between inventory tiles
#define BORDER_THICKNESS 4 // Thicker border

static TTF_Font *font = NULL;

int initialize_font(const char *font_path, int font_size)
{
    font = TTF_OpenFont(font_path, font_size);
    if (!font)
    {
        SDL_Log("TTF_OpenFont() Error: %s", SDL_GetError());
        return -1;
    }
    return 0;
}

void render_text(SDL_Renderer *renderer, const char *text, SDL_Color color, float x, float y, float w, float h)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, strlen(text), color);
    if (!surface)
    {
        SDL_Log("TTF_RenderText_Blended() Error: %s", SDL_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture)
    {
        SDL_Log("SDL_CreateTextureFromSurface() Error: %s", SDL_GetError());
        return;
    }

    SDL_FRect dstRect = {x, y, w, h};
    SDL_RenderTexture(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

void cleanup_font()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
}

void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height, int inventory_y, int item_offset)
{
    int inventory_width = INVENTORY_SIZE * (ITEM_SIZE + PADDING) - PADDING;
    int inventory_x = (screen_width - inventory_width) / 2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for borders

    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        SDL_FRect item_rect = {inventory_x + i * (ITEM_SIZE + PADDING), inventory_y, ITEM_SIZE, ITEM_SIZE};
        SDL_RenderRect(renderer, &item_rect);

        if (item_offset == 0 && i == inventory_selection->selected_main_item)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                                                                                                                // Red color for selected main item border
            SDL_FRect highlight_rect = {item_rect.x - BORDER_THICKNESS, item_rect.y - BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS}; // Thicker border
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Reset to white color
        }
        else if (item_offset == 16 && i == inventory_selection->selected_aux_item - 16)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);                                                                                                                // Green color for selected seed
            SDL_FRect highlight_rect = {item_rect.x - BORDER_THICKNESS, item_rect.y - BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS}; // Thicker border
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Reset to white color
        }
        else if (item_offset == 32 && i == inventory_selection->selected_aux_item - 32)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);                                                                                                                // Blue color for selected harvest
            SDL_FRect highlight_rect = {item_rect.x - BORDER_THICKNESS, item_rect.y - BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS, ITEM_SIZE + 2 * BORDER_THICKNESS}; // Thicker border
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Reset to white color
        }

        // Render item from item_tilemap
        SDL_FRect src_rect = {i * 16, item_offset, 16, 16}; // Source size is 16x16
        SDL_RenderTexture(renderer, item_tilemap, &src_rect, &item_rect);
    }
}

void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height)
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

    int inventory_y = screen_height - ITEM_SIZE - 20; // 20 pixels from the bottom
    render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y, 0);

    // Render additional inventories for seed pouch and harvest bag
    if (inventory_selection->selected_main_item == 3) // Seed pouch
    {
        inventory_selection->selected_aux_inventory = 1;
        render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y - ITEM_SIZE - 20, 16); // Offset for seeds
    }
    else if (inventory_selection->selected_main_item == 4) // Harvest bag
    {
        inventory_selection->selected_aux_inventory = 2;
        render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y - ITEM_SIZE - 20, 32); // Offset for harvested products
    }
    else
    {
        inventory_selection->selected_aux_inventory = 0;
    }
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

    // Check for additional inventories
    if (y >= inventory_y - ITEM_SIZE - 20 && y <= inventory_y - 20)
    {
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            int slot_x = inventory_x + i * (ITEM_SIZE + PADDING);
            if (x >= slot_x && x <= slot_x + ITEM_SIZE)
            {
                *slot = i + INVENTORY_SIZE; // Offset for additional inventory
                return true;
            }
        }
    }

    return false;
}

void handle_seed_harvest_selection(int *selected_item, int inventory_size, int offset)
{
    if (*selected_item >= offset && *selected_item < offset + inventory_size)
    {
        *selected_item = (*selected_item - offset + inventory_size) % inventory_size + offset;
    }
    else
    {
        *selected_item = offset;
    }
}

void handle_aux_selection(int *selected_aux_item, int inventory_size, int offset)
{
    if (*selected_aux_item >= offset && *selected_aux_item < offset + inventory_size)
    {
        *selected_aux_item = (*selected_aux_item - offset + inventory_size) % inventory_size + offset;
    }
    else
    {
        *selected_aux_item = offset;
    }
}

// ...existing code...