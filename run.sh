#!/bin/bash

# Navigate to the DLLs directory
cd DLLs

# Remove the old executable
rm -v main.exe

# Compile the new executable
gcc -v -I../libs/include -L../libs/lib -o main.exe ../src/main.c ../src/window/init.c ../src/window/grid.c ../src/window/texture.c ../src/window/event_loop.c ../src/window/ui.c -lSDL3 -lSDL3_image

# Run the new executable
./main.exe
