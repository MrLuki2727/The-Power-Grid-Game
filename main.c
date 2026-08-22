#include <stdio.h>
#include "raylib.h"
#include "render.h"
#include "simulation.h"


#define SIMULATION_INTERVAL 0.5f // 500ms

int main(void)
{
    GameState game_state;
    simulation_init(&game_state);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(900, 650, "Power Grid Game");
    SetTargetFPS(60);
    load_textures();
    float sim_timer = 0.0f;
    while (!WindowShouldClose())
    {
        if (!is_shop_open)
        {
            float delta_time = GetFrameTime();
            sim_timer += delta_time;
        }
        if (sim_timer >= SIMULATION_INTERVAL)
        {
            simulation_update(&game_state, SIMULATION_INTERVAL);
            sim_timer -= SIMULATION_INTERVAL;
        }


        BeginDrawing();
        ClearBackground(BLACK);


        render_top_zone(&game_state);
        render_dashboard(&game_state);
        render_stability_gauge(&game_state);

        if (is_shop_open)
        {
            render_shop(&game_state);
        }
        EndDrawing();
    }
}
