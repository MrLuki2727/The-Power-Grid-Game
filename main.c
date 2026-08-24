#include <stdio.h>
#include "raylib.h"
#include "render.h"
#include "simulation.h"


#define SIMULATION_INTERVAL 0.7f // 500ms

int main(void)
{
    GameState game_state;
    simulation_init(&game_state);


    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(900, 650, "Power Grid Game");
    //SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    load_textures();

    float sim_timer = 0.0f;
    while (!WindowShouldClose())
    {
        if (!is_popup_open)
        {
            float delta_time = GetFrameTime();
            sim_timer += delta_time;
        }
        if (sim_timer >= SIMULATION_INTERVAL)
        {
            simulation_update(&game_state, SIMULATION_INTERVAL);
            sim_timer -= SIMULATION_INTERVAL;
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            is_popup_open = !is_popup_open;
            if (is_shop_open)
            {
                is_shop_open = false;

            }
            else if (is_settings_open)
            {
                is_settings_open = false;
                is_menu_open = true;
                popup_just_opened = true;

            }
            else if (is_save_game_open)
            {
                is_save_game_open = false;
                is_menu_open = true;
                popup_just_opened = true;
            }
            else if (is_load_game_open)
            {
                is_load_game_open = false;
                is_menu_open = true;
                popup_just_opened = true;
            }
            else
            {
             is_menu_open = !is_menu_open;
            }


        }


        BeginDrawing();
        ClearBackground(BLACK);
        render_top_zone(&game_state);
        render_dashboard(&game_state);
        render_stability_gauge(&game_state);
        render_power_graph(&game_state);
        render_day_night_indicator(&game_state);

        if (is_shop_open)
        {
            render_shop(&game_state);
        }
        if (is_settings_open)
        {
            render_settings(&game_state);
        }
        if (is_menu_open)
        {
            render_menu(&game_state);
        }
        if (is_save_game_open)
        {
            render_save_game_popup(&game_state);
        }
        if (is_load_game_open)
        {
            render_load_game_popup(&game_state);
        }
        EndDrawing();
        popup_just_opened = false;
    }
}
