
#include "window.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

void save_game_state(const char *filename, int character_x, int character_y)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        SDL_Log("Error opening save file: %s", strerror(errno));
        return;
    }
    fprintf(file, "%d %d\n", character_x, character_y);
    fclose(file);
}

void load_game_state(const char *filename, int *character_x, int *character_y)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_Log("Save file not found, creating a new one.");
        *character_x = 0;
        *character_y = 0;
        save_game_state(filename, *character_x, *character_y);
        return;
    }
    fscanf(file, "%d %d", character_x, character_y);
    fclose(file);
}