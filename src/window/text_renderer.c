#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

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