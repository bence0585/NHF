#!/bin/bash

# Navigate to the DLLs directory
cd DLLs

# Remove the old executable
rm -v main.exe

# Compile the new executable
gcc -I../libs/include -L../libs/lib -Wall -Werror -o main.exe ../src/main.c ../src/window/init.c ../src/window/grid.c ../src/window/texture.c ../src/window/event_loop.c ../src/window/ui.c ../src/window/text_renderer.c ../src/window/crop.c ../src/window/game_state.c ../src/window/tool.c -lSDL3 -lSDL3_image -lSDL3_ttf 

# Run the new executable
./main.exe
