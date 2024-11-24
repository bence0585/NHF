#include "window.h"
#include <stdlib.h>
#include <time.h>

void initialize_crop_manager(CropManager *crop_manager)
{
    crop_manager->crops = NULL;
    crop_manager->crop_count = 0;
    srand(time(NULL)); // Initialize random seed
}

void add_crop(CropManager *crop_manager, int x, int y, CropType type, int growth_time)
{
    crop_manager->crop_count++;
    crop_manager->crops = realloc(crop_manager->crops, crop_manager->crop_count * sizeof(Crop));
    crop_manager->crops[crop_manager->crop_count - 1] = (Crop){x, y, type, CROP_PHASE_1, growth_time, 0};
}

void update_crops(CropManager *crop_manager, int ticks, Grid *grid)
{
    for (int i = 0; i < crop_manager->crop_count; i++)
    {
        Crop *crop = &crop_manager->crops[i];
        int growth_chance = (get_tile_type(grid, crop->x, crop->y) == TILE_WATERED) ? 80 : 30; // 80% if watered, 30% if not

        if (rand() % 100 < growth_chance) // Chance to count the tick towards growth
        {
            crop->current_time += ticks;
            if (crop->current_time >= crop->growth_time && crop->growth_stage < crop_info[crop->type].growth_stages - 1)
            {
                crop->growth_stage++;
                crop->current_time = 0;
            }
            if (crop->growth_stage == crop_info[crop->type].growth_stages - 1)
            {
                crop->current_time = 0; // Ensure the last growth stage is set to the max stage minus one with a current time of 1
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

        const CropInfo *info = &crop_info[crop->type];
        int growth_stage = (crop->growth_stage >= info->growth_stages ? info->growth_stages - 1 : crop->growth_stage);
        int src_x = ((info->texture_start % 16) + growth_stage) * tile_size;
        int src_y = (info->texture_start / 16) * tile_size - tile_size;

        SDL_FRect src = {src_x, src_y, tile_size, 2 * tile_size}; // Source texture is 16x32
        SDL_RenderTexture(renderer, crop_texture, &src, &dest);
    }
}