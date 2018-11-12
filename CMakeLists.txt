cmake_minimum_required(VERSION 3.12)
project(sdl_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra") #  -Wall -Wextra

add_executable(sdl_game src/main.cpp)

include(FindPkgConfig)

pkg_search_module(SDL2      REQUIRED sdl2>=2.0.0)
pkg_search_module(SDL2TTF   REQUIRED SDL2_ttf>=2.0.0)
pkg_search_module(SDL2NET   REQUIRED SDL2_net>=2.0.0)
pkg_search_module(SDL2GFX   REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_subdirectory("${PROJECT_SOURCE_DIR}/../sdl_oo" "${PROJECT_SOURCE_DIR}/../sdl_oo/cmake-build-debug")

include_directories(
        ${PROJECT_SOURCE_DIR}/../sdl_oo/include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
        ${SDL2NET_INCLUDE_DIRS}
        ${SDL2GFX_INCLUDE_DIRS}
        ${SDL2MIXER_INCLUDE_DIRS}
        ${SDL2IMAGE_INCLUDE_DIRS})

target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
        ${SDL2NET_LIBRARIES}
        ${SDL2GFX_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES})
