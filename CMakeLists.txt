cmake_minimum_required(VERSION 3.27)
project(Solitaire C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/ziabe/Documents/sdl/SDL2-2.30.2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(Solitaire
        Main.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
