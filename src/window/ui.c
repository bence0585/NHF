#include "window.h"
#include <stdio.h>
#include <SDL3_ttf/SDL_ttf.h>

static const int inventory_size = 9;
static const int padding = 4;
static const int item_size = 64;
static TTF_Font *font = NULL;

/*
 * Inicializálja a betűtípust
 * @param font_path a betűtípus elérési útja
 * @param font_size a betűméret
 * @return 0, ha sikeres, -1, ha sikertelen
 * @note A font változó globális változó
 */
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

/*
 * Kirajzol egy szöveget a megadott helyre és méretben a megadott színnel és betűtípussal
 * @param renderer a rajzoló
 * @param text a szöveg
 * @param color a szín
 * @param x x koordináta
 * @param y y koordináta
 * @param w szélesség
 * @param h magasság
 * @return void
 */
void render_text(SDL_Renderer *renderer, const char *text, SDL_Color color, float x, float y, float w, float h)
{
    // Háttér kirajzolása a szöveg mögé
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 10);            // Fekete szín 10-es átlátszósággal
    SDL_FRect background_rect = {x - 2, y - 2, w + 4, h + 4}; // A szövegnél egy kicsit nagyobb
    SDL_RenderFillRect(renderer, &background_rect);           // Háttér kirajzolása

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, strlen(text), color); // Szöveg felület létrehozása
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
/*
 * Betűtípus felszabadítása
 * @return void
 */
void cleanup_font()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
}
/*
 * A karakterláda kirajzolása
 * @param renderer a rajzoló
 * @param item_tilemap a tárgyak textúrája
 * @param inventory_selection a kiválasztott tárgyak
 * @param screen_width a képernyő szélessége
 * @param screen_height a képernyő magassága
 * @param inventory_y az karakterláda y koordinátája, a képernyő aljától mérve
 * @param item_offset a tárgyak eltolása a tilemapon
 * @return void
 */
void render_inventory(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height, int inventory_y, int item_offset)
{
    int inventory_width = inventory_size * (item_size + padding) - padding;
    int inventory_x = (screen_width - inventory_width) / 2;
    int border_thickness = 4;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for borders

    for (int i = 0; i < inventory_size; i++)
    {
        SDL_FRect item_rect = {inventory_x + i * (item_size + padding), inventory_y, item_size, item_size};
        SDL_RenderRect(renderer, &item_rect);

        if (item_offset == 0 && i == inventory_selection->selected_main_item)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                                                                                                                // Vörös szín a kiválasztott fő tárgyakhoz
            SDL_FRect highlight_rect = {item_rect.x - border_thickness, item_rect.y - border_thickness, item_size + 2 * border_thickness, item_size + 2 * border_thickness}; // Vastagabb keret
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Viszaállítás fehér színre
        }
        else if (item_offset == 16 && i == inventory_selection->selected_aux_item - 16)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);                                                                                                                // Zöld szín a kiválasztott vetőmagokhoz
            SDL_FRect highlight_rect = {item_rect.x - border_thickness, item_rect.y - border_thickness, item_size + 2 * border_thickness, item_size + 2 * border_thickness}; // Vastagabb keret
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Visszaállítás fehér színre
        }
        else if (item_offset == 32 && i == inventory_selection->selected_aux_item - 32)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);                                                                                                                // Kék szín a kiválasztott betakarított termékekhez
            SDL_FRect highlight_rect = {item_rect.x - border_thickness, item_rect.y - border_thickness, item_size + 2 * border_thickness, item_size + 2 * border_thickness}; // Vastagabb keret
            SDL_RenderFillRect(renderer, &highlight_rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Visszaállítás fehér színre
        }

        // Tárgy kirajzolása a tilemapról
        SDL_FRect src_rect = {i * 16, item_offset, 16, 16}; //  16x16 méretű forrás téglalap a tilemapon
        SDL_RenderTexture(renderer, item_tilemap, &src_rect, &item_rect);

        // Számláló kirajzolása a mellék tárgyakhoz
        if (item_offset == 16 || item_offset == 32)
        {
            char count_text[4];
            int count = (item_offset == 16) ? inventory_selection->seed_counts[i] : inventory_selection->harvest_counts[i];
            snprintf(count_text, sizeof(count_text), "%d", count);
            SDL_Color white = {255, 255, 255, 255};
            render_text(renderer, count_text, white, item_rect.x + 5, item_rect.y + 5, 20, 20);
        }
    }
}

/*
 * A felhasználói felület kirajzolása
 * @param renderer a rajzoló
 * @param item_tilemap a tárgyak textúrája
 * @param inventory_selection a kiválasztott tárgyak
 * @param screen_width a képernyő szélessége
 * @param screen_height a képernyő magassága
 * @return void
 */
void render_ui(SDL_Renderer *renderer, SDL_Texture *item_tilemap, InventorySelection *inventory_selection, int screen_width, int screen_height)
{
    SDL_FRect zoom_in_button = {10.0f, 10.0f, 60.0f, 60.0f}; // Gomb helye, mérete
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);        // Zöld szín
    SDL_RenderFillRect(renderer, &zoom_in_button);

    SDL_FRect zoom_out_button = {80.0f, 10.0f, 60.0f, 60.0f}; // Gomb helye, mérete
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);         // Vörös szín
    SDL_RenderFillRect(renderer, &zoom_out_button);

    SDL_FRect save_game_button = {150.0f, 10.0f, 60.0f, 60.0f}; // Gomb helye, mérete
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);           // Kék szín
    SDL_RenderFillRect(renderer, &save_game_button);

    // Pénzszámláló kirajzolása
    char money_text[20];
    snprintf(money_text, sizeof(money_text), "Péz: %d", inventory_selection->money);
    SDL_Color white = {255, 255, 255, 255};
    render_text(renderer, money_text, white, 10.0f, 80.0f, 200.0f, 30.0f); // Zoom gombok alá pozícionálva

    int inventory_y = screen_height - item_size - 20; // 20 pixellel a képernyő aljától
    render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y, 0);

    // További, aux karakterládák kirajzolása
    if (inventory_selection->selected_main_item == 3) // Vetőmag tároló
    {
        inventory_selection->selected_aux_inventory = 1;
        render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y - item_size - 20, 16); // UA mint a fő karakterláda, de mivel egy képen van tárolva a tilemap, ezért az eltolás szükséges, itt 16 px
    }
    else if (inventory_selection->selected_main_item == 4) // Betakarított termékek tároló
    {
        inventory_selection->selected_aux_inventory = 2;
        render_inventory(renderer, item_tilemap, inventory_selection, screen_width, screen_height, inventory_y - item_size - 20, 32); // UA mint a fő karakterláda, de mivel egy képen van tárolva a tilemap, ezért az eltolás szükséges, itt 32 px
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

/*
 * Megvizsgálja, hogy a gombra kattintottak-e
 * @param button a gomb típusa
 * @param x az egér x koordinátája
 * @param y az egér y koordinátája
 * @return true, ha a gombra kattintottak, egyébként false
 */
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

/*

    * Megvizsgálja, hogy az inventárió slotra kattintottak-e
    * @param x az egér x koordinátája
    * @param y az egér y koordinátája
    * @param screen_width a képernyő szélessége
    * @param screen_height a képernyő magassága
    * @param slot az inventárió slot indexe
    * @return true, ha az inventárió slotra kattintottak, egyébként false
*/
bool is_inventory_slot_clicked(int x, int y, int screen_width, int screen_height, int *slot)
{
    int inventory_width = inventory_size * (item_size + padding) - padding;
    int inventory_x = (screen_width - inventory_width) / 2;
    int inventory_y = screen_height - item_size - 20; // UA, mint a karakterláda pozíciója

    if (y >= inventory_y && y <= inventory_y + item_size)
    {
        for (int i = 0; i < inventory_size; i++)
        {
            int slot_x = inventory_x + i * (item_size + padding);
            if (x >= slot_x && x <= slot_x + item_size)
            {
                *slot = i;
                return true;
            }
        }
    }

    // Az aux karakterláda slotjainak vizsgálata
    if (y >= inventory_y - item_size - 20 && y <= inventory_y - 20)
    {
        for (int i = 0; i < inventory_size; i++)
        {
            int slot_x = inventory_x + i * (item_size + padding);
            if (x >= slot_x && x <= slot_x + item_size)
            {
                *slot = i + inventory_size; // Az aux karakterláda slotjainak indexelése
                return true;
            }
        }
    }

    return false;
}
/*
 * Az aux karakterláda kiválasztásának kezelése
 * @param selected_aux_item az aux karakterláda kiválasztott tárgyának indexe
 * @param inventory_size az inventárió mérete
 * @param offset az aux karakterláda slotjainak eltolása
 * @return void
 * @note Az aux karakterláda kiválasztott tárgyának indexelése az eltolás figyelembevételével
 */
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
