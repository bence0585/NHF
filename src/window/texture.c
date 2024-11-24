#include "window.h"
#include <SDL3/SDL_surface.h>
#include <SDL3_Image/SDL_image.h>

/*
 * Betölt egy textúrát a megadott fájlból
 * @param renderer a kirajzolást végző renderer
 * @param file_path a fájl útvonala
 * @return az SDL_Texture mutatója, vagy NULL, ha sikertelen
 */
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file_path)
{
    SDL_Surface *surface = IMG_Load(file_path);
    if (!surface)
    {
        SDL_Log("Error loading PNG: %s", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}
