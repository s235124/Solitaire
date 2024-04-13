cmake_minimum_required(VERSION 3.27)
project(Solitaire C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)


add_executable(Solitaire
        Main.c
)
