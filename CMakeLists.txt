cmake_minimum_required(VERSION 3.10)

# Set the project name and specify C as the language
project(Tetris C)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable all warnings
add_compile_options(-Wall)

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)
# Fetch raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        c1ab645ca298a2801097931d1079b10ff7eb9df8
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   ON
)
FetchContent_MakeAvailable(raylib)

# Add the executable target
add_executable(Tetris main.c board.c pallet_texture.c tetromino.c texture_store.c score.c)

target_link_libraries(Tetris raylib)
