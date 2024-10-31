cmake_minimum_required(VERSION 3.10)
project(NHF)

set(CMAKE_C_STANDARD 11)

# Include directories and libraries
include_directories(libs/include)
link_directories(libs/lib)

# Add executable and source files
add_executable(main src/main.c src/window/init.c src/window/grid.c src/window/texture.c src/window/event_loop.c src/window/ui.c)

# Link libraries
target_link_libraries(main SDL3 SDL3_image)
