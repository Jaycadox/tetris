cmake_minimum_required(VERSION 3.10)

# Set the project name and specify C as the language
project(Minesweeper C)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable all warnings
add_compile_options(-Wall)

add_subdirectory(raylib)

# Add the executable target
add_executable(Tetris main.c board.c board.h pallet_texture.c pallet_texture.h score.c score.h tetromino.c tetromino.h texture_store.c texture_store.h)

target_link_libraries(Tetris raylib)
