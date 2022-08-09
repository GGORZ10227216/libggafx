cmake_minimum_required(VERSION 3.15)
project(libgafx)

set(CMAKE_CXX_FLAGS  "--std=c++2a")
set(CMAKE_CXX_FLAGS_DEBUG "-g ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")

find_package(PkgConfig REQUIRED) 
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(FMT REQUIRED fmt)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS)

file(GLOB IMGUI_SRC imgui/*.cpp)
file(GLOB SRC src/*.cpp)

set(IMGUI_BACKEND_OGL_SDL imgui/backends/imgui_impl_sdl.cpp imgui/backends/imgui_impl_opengl3.cpp)
# set(IMFILE_SRC imgui_3rdParty/ImFileDialog/ImFileDialog.cpp)
set(GGTOOL_SRC imgui_3rdParty/imgui_grid_image/imgui_grid_image.cpp)

set(GGTOOL_SRC
        src/ggfxtool/gg_abstract_widget.cpp
        src/ggfxtool/gg_texture.cpp
        include/ggfxtool/gg_palette_viewer.h
        src/ggfxtool/gg_palette_viewer.cpp
        src/videodump_viewer.cpp
        src/ggfxtool/ppu/ppu.cpp
        src/ggfxtool/gg_tile_viewer.cpp
        src/ggfxtool/gg_background_viewer.cpp
        src/ggfxtool/gg_sprite_viewer.cpp
        src/ggfxtool/ppu/text_background.cpp
        src/ggfxtool/ppu/affine_background.cpp
        src/ggfxtool/gg_sprite_viewer.cpp
        )

set(SRC main.cpp ${IMGUI_SRC} ${IMGUI_BACKEND_OGL_SDL} ${GGTOOL_SRC} ${SRC} glad/src/glad.c imgui_3rdParty/imgui_grid_image/imgui_grid_image.cpp imgui_3rdParty/imgui_grid_image/imgui_grid_image.h src/ggfxtool/ppu/sprite.cpp)

add_executable(libgafx ${SRC})

include_directories(libgafx PUBLIC ${CMAKE_CURRENT_LIST_DIR}
        glad/include ${SDL2_INCLUDE_DIRS} imgui imgui/backends include include/ggfxtool imgui_3rdParty/imgui_memory_editor imgui_3rdParty/imgui_grid_image/)

target_link_libraries(libgafx ${SDL2_LDFLAGS} ${FMT_LDFLAGS} -lpthread)

