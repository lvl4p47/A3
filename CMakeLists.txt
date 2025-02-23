# необходимая минимальная версия cmake
cmake_minimum_required(VERSION 3.16)
# даем название проекта и задаем версию сборки
project(A3 VERSION 0.1)
# указываем что проект собирается на основе си кода 11 стандарта
set(CMAKE_C_STANDARD 11)

# задаем переменную SOURCES как набор твоих исходников
set(SOURCES src/main.c src/func.c src/hexagons.c src/input.c src/main.c src/text.c src/video.c src/tiles.png src/font.png src/ui.txt src/interface.c)

# задаем цель сборки под именем main и указываем исходники
add_executable(main ${SOURCES})

add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_BUILD_TYPE Debug)

# говорим cmake что надо будет линковать бибку к нашей цели main
target_link_libraries(main SDL2-2.0 SDL2_image-2.0)
