gcc -Ilibs/include -Llibs/lib -o src/bin/main.exe src/main.c src/window/init.c src/window/grid.c src/window/texture.c src/window/event_loop.c src/window/ui.c -lSDL3 -lSDL3_image && ./src/bin/main.exe
