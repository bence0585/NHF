
#include "window.h"

void game_tick(CropManager *crop_manager, int ticks)
{
    update_crops(crop_manager, ticks);
}