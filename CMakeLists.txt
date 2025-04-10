cmake_minimum_required(VERSION 3.10)

project(main)

add_compile_options(-Wall -Wextra -Wpedantic)

set(SOURCES src/main.c src/func.c src/hexagons.c src/input.c src/main.c src/text.c src/video.c media/tiles.png media/ui_tiles.png media/font.png media/ui.txt src/interface.c src/audio.h src/audio.c src/entity.h src/entity.c)

add_executable(main ${SOURCES})

set(CMAKE_BUILD_TYPE Release)

target_link_libraries(main SDL2 SDL2_image SDL2_mixer)
