cmake_minimum_required(VERSION 4.3)
project(Power_Game C)

set(CMAKE_C_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(Power_Game main.c
        simulation.c simulation.h
        render.c render.h
        plant.c plant.h
        embedded_images.h)


target_link_libraries(Power_Game raylib)

if (WIN32)
    target_link_options(Power_Game PRIVATE -mwindows)
endif()