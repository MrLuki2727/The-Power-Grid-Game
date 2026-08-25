#include <stdio.h>
#include "raylib.h"
#include "render.h"
#include "simulation.h"
#include "embedded_images.h"


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
    Image icon = LoadImageFromMemory(".png", logo_png, logo_png_size);
    SetWindowIcon(icon);
    if (icon.data == NULL || icon.width == 0) {
        TraceLog(LOG_ERROR, "ICON FEHLER: Bild konnte nicht geladen werden! Size: %u", logo_png_size);
    } else {
        TraceLog(LOG_INFO, "ICON SUCCESS: Geladen mit %dx%d Pixel", icon.width, icon.height);
        SetWindowIcon(icon);
    }
    UnloadImage(icon);


    float sim_timer = 0.0f;
    float title_time = 0.0f;
    while (!WindowShouldClose() && !quit_game)
    {
        float delta_time = GetFrameTime();

        if (!is_popup_open && game_state.is_playing)
        {

            sim_timer += delta_time;

            if (sim_timer >= SIMULATION_INTERVAL)
            {
                simulation_update(&game_state, SIMULATION_INTERVAL);
                sim_timer -= SIMULATION_INTERVAL;
            }
        }
        if (IsKeyPressed(KEY_ESCAPE) && game_state.is_playing)
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
            else if (is_upgrade_open)
            {
                is_upgrade_open = false;
            }
            else
            {
             is_menu_open = !is_menu_open;
            }


        }


        BeginDrawing();
        ClearBackground(BLACK);
        if (game_state.is_playing)
        {
            render_top_zone(&game_state);
            render_dashboard(&game_state);
            render_stability_gauge(&game_state);
            render_power_graph(&game_state);
            render_day_night_indicator(&game_state);
        }
        else
        {
            title_time += delta_time;
            render_title_screen(&game_state, title_time);
        }
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
        if (is_upgrade_open)
        {
            render_upgrade_popup(&game_state);
        }
        EndDrawing();
        popup_just_opened = false;
    }
}
