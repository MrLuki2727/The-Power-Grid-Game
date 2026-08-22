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
        simulation.c
        simulation.h
        render.c
        render.h
        plant.c
        plant.h)

target_link_libraries(Power_Game raylib)

add_custom_command(TARGET Power_Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Pictures
        $<TARGET_FILE_DIR:Power_Game>/Pictures
)