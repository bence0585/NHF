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
}

/*
 * Megadja, hogy a nagyítás gombra kattintottak-e
 * @param x: az egér x koordinátája
 * @param y: az egér y koordinátája
 * @return: igaz, ha a gombra kattintottak, egyébként hamis
 */
bool is_zoom_in_button_clicked(int x, int y)
{
    return (x >= 10 && x <= 60 && y >= 10 && y <= 60);
}

/*
 * Megadja, hogy a kicsinyítés gombra kattintottak-e
 * @param x: az egér x koordinátája
 * @param y: az egér y koordinátája
 * @return: igaz, ha a gombra kattintottak, egyébként hamis
 */
bool is_zoom_out_button_clicked(int x, int y)
{
    return (x >= 70 && x <= 120 && y >= 10 && y <= 60);
}