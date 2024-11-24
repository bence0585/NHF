#include "window.h"
#include <stdlib.h>
#include <time.h>

void initialize_crop_manager(CropManager *crop_manager)
{
    crop_manager->crops = NULL;
    crop_manager->crop_count = 0;
    srand(time(NULL)); // Initialize random seed
}

void add_crop(CropManager *crop_manager, int x, int y, int growth_time)
{
    crop_manager->crop_count++;
    crop_manager->crops = realloc(crop_manager->crops, crop_manager->crop_count * sizeof(Crop));
    crop_manager->crops[crop_manager->crop_count - 1] = (Crop){x, y, CROP_PHASE_1, growth_time, 0};
}

void update_crops(CropManager *crop_manager, int ticks)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        if (rand() % 100 < 80) // 80% chance to count the tick towards growth
        {
            crop->current_time += ticks;
            if (crop->current_time >= crop->growth_time && crop->growth_stage < CROP_PHASE_MAX)
            {
                crop->growth_stage++;
                crop->current_time = 0;
            }
        }
    }
}

void render_crops(SDL_Renderer *renderer, SDL_Texture *crop_texture, CropManager *crop_manager, int tile_size, double zoom_level, int offset_x, int offset_y)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        int crop_height = 2 * tile_size * zoom_level; // Always render as 2 tall
        SDL_FRect dest = {offset_x + crop->x * tile_size * zoom_level, offset_y + (crop->y - 1) * tile_size * zoom_level, tile_size * zoom_level, crop_height};
        SDL_FRect src = {crop->growth_stage * tile_size, 0, tile_size, 2 * tile_size}; // Source texture is 16x32
        SDL_RenderTexture(renderer, crop_texture, &src, &dest);
    }
}