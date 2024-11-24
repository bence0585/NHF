#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "../debugmalloc.h"

/*
 * Inicializálja az SDL-t videó és események kezelésére.
 * @return 0, ha sikeres, -1, ha sikertelen
 */
int initialize_SDL()
{
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result < 0)
    {
        SDL_Log("SDL_Init hiba: %s", SDL_GetError());
    }
    return result;
}

/*
 * Inicializálja az SDL_ttf könyvtárat.
 * @return 0, ha sikeres, -1, ha sikertelen
 */
int initialize_SDLTTF()
{
    int result = TTF_Init();
    if (result < 0)
    {
        SDL_Log("TTF_Init() hiba: %s", SDL_GetError());
        return -1;
    }
    return 0;
}

/*
 * Létrehoz egy SDL ablakot a megadott címmel, szélességgel és magassággal.
 * @param title az ablak címe
 * @param width az ablak szélessége
 * @param height az ablak magassága
 * @return az SDL_Window mutatója, vagy NULL, ha sikertelen
 */
SDL_Window *create_window(const char *title, int width, int height)
{
    SDL_Window *window = SDL_CreateWindow(title, width, height, 0);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow hiba: %s", SDL_GetError());
    }
    return window;
}

/*
 * Létrehoz egy SDL renderert a megadott ablakhoz.
 * @param window az SDL_Window mutatója
 * @return az SDL_Renderer mutatója, vagy NULL, ha sikertelen
 */
SDL_Renderer *create_renderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer hiba: %s", SDL_GetError());
    }
    return renderer;
}

/*
 * Tisztítja az SDL erőforrásokat, beleértve a renderert és az ablakot.
 * @param renderer az SDL_Renderer mutatója
 * @param window az SDL_Window mutatója
 * @return void
 */
void cleanup(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}