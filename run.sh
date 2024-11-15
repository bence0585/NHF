#!/bin/bash



cd DLLs && gcc -I../libs/include -L../libs/lib -o main.exe ../src/main.c ../src/window/init.c ../src/window/grid.c ../src/window/texture.c ../src/window/event_loop.c ../src/window/ui.c -lSDL3 -lSDL3_image && ./main.exe
