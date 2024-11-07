#!/bin/bash

# Change to the directory where the script is located
cd "$(dirname "$0")"

# Change to the parent directory to ensure relative paths work
cd ..

# Compile the project
gcc -Ilibs/include -Llibs/lib -o src/bin/main.exe src/main.c src/window/init.c src/window/grid.c src/window/texture.c src/window/event_loop.c src/window/ui.c -lSDL3 -lSDL3_image

# Run the executable
./src/bin/main.exe