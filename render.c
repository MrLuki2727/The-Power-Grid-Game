#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include <stdio.h>
#include "plant.h"
#include "embedded_images.h"

int target_plant = 0;
bool is_popup_open = false;
bool popup_just_opened = false;

bool is_shop_open = false;
bool is_menu_open = false;
bool is_settings_open = false;
bool is_save_game_open = false;
bool is_load_game_open = false;
bool is_upgrade_open = false;

bool quit_game = false;



Texture2D plant_icons[8];

void load_textures(void)
{
    Image img;

    img = LoadImageFromMemory(".png", no_power_plant_png, no_power_plant_png_size);
    plant_icons[0] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", solar_png, solar_png_size);
    plant_icons[1] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", wind_png, wind_png_size);
    plant_icons[2] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", water_power_plant_png, water_power_plant_png_size);
    plant_icons[3] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", pump_power_plant_png, pump_power_plant_png_size);
    plant_icons[4] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", battery_png, battery_png_size);
    plant_icons[5] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", coal_power_plant_png, coal_power_plant_png_size);
    plant_icons[6] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImageFromMemory(".png", nuclear_power_plant_png, nuclear_power_plant_png_size);
    plant_icons[7] = LoadTextureFromImage(img);
    UnloadImage(img);

}


#define min(a,b) ((a) < (b) ? (a) : (b))

void render_top_zone(GameState *game_state)
{


    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    int screen_w = GetRenderWidth();
    int screen_h = GetRenderHeight();
    int top_zone_height = screen_h * 0.8f;
    int margin = 10;
    int col_width = (screen_w - margin * 5) / 4;  // 4 Spalten
    int row_height = (top_zone_height - margin * 3) / 2; // 2 Reihen

    for (int i = 0; i < 8; i++)
    {
        int col = i % 4;
        int row = i / 4;

        Rectangle slot = {
            margin + col * (col_width + margin),
            margin + row * (row_height + margin),
            col_width,
            row_height
        };

        Color slot_color = LIGHTGRAY;

        if ( is_regulatable(game_state->Power_plants[i].type) && game_state->Power_plants[i].aimed_power_generation > 0) slot_color = GREEN;
        else if (is_regulatable(game_state->Power_plants[i].type) && game_state->Power_plants[i].aimed_power_generation == 0) slot_color = RED;
        else if (game_state->Power_plants[i].status == 1) slot_color = GREEN;
        else if (game_state->Power_plants[i].status == 0 && game_state->Power_plants[i].type == 0) slot_color = GRAY;
        else if (game_state->Power_plants[i].status == 0) slot_color = RED;
        else if (game_state->Power_plants[i].status == 2) slot_color = YELLOW;
        else if (game_state->Power_plants[i].status == 3) slot_color = BLUE;
        DrawRectangleRec(slot, slot_color);

        int plant_info_text_size = screen_h * 0.02f;
        int button_hight = screen_h * 0.04f;

        switch (game_state->Power_plants[i].type)
        {
            case 0:
                {
                    DrawText(TextFormat("No Power Plant"), slot.x + 5, slot.y + 5, plant_info_text_size, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, min(game_state->Power_plants[i].power_max,game_state->Power_plants[i].aimed_power_generation)),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);

                    Rectangle buy_plant_button = { slot.x, slot.y + row_height - 30, col_width, 30 };
                    if (GuiButton(buy_plant_button, "Buy Plant") && !is_popup_open)
                    {
                        popup_just_opened = true;
                        is_shop_open = true;
                        target_plant = i;
                    }
                    break;
                }
            case 1:
                {
                    DrawText(TextFormat("Solar"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    break;
                }
            case 2:
                {
                    DrawText(TextFormat("Wind Turbine"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    break;
                }
            case 3:
                {
                    DrawText(TextFormat("Water"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    break;
                }
            case 4:
                {
                    DrawText(TextFormat("Pump"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %d kW | %d kWh / %d kWh", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max,game_state->Power_plants[i].capacity,game_state->Power_plants[i].max_capacity),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(start_plant_button, "Pump") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width)/4, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(stop_plant_button, "Stop") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    Rectangle regenerate_plant_button = { slot.x + (col_width)/2, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(regenerate_plant_button, "Start") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 2;
                    }
                    Rectangle auto_plant_button = { slot.x + ((col_width)/4)*3, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(auto_plant_button, "Auto") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 3;
                    }
                    break;
                }
            case 5:
                {
                    DrawText(TextFormat("Battery"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %d kW | %d kWh / %d kWh", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max,game_state->Power_plants[i].capacity,game_state->Power_plants[i].max_capacity),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(start_plant_button, "Charge") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width)/4, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(stop_plant_button, "Stop") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    Rectangle regenerate_plant_button = { slot.x + (col_width)/2, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(regenerate_plant_button, "Start") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 2;
                    }
                    Rectangle auto_plant_button = { slot.x + ((col_width)/4)*3, slot.y + row_height - 30, (col_width) / 4, 30 };
                    if (GuiButton(auto_plant_button, "Auto") && !is_popup_open)
                    {
                        game_state->Power_plants[i].status = 3;
                    }
                    break;
                }
            case 6:
                {
                    DrawText(TextFormat("Coal"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };


                    float slider_value = (float)game_state->Power_plants[i].aimed_power_generation;
                    Rectangle slider_rect = { slot.x + 40, slot.y + row_height - 30, col_width - 100, 20 };
                    GuiSlider(slider_rect, "0%", "100%", &slider_value, 0.0f, game_state->Power_plants[i].power_max);
                    game_state->Power_plants[i].aimed_power_generation = (int)slider_value;
                    break;
                }
            case 7:
                {
                    DrawText(TextFormat("Atom Plant"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);

                    float slider_value = (float)game_state->Power_plants[i].aimed_power_generation;
                    Rectangle slider_rect = { slot.x + 40, slot.y + row_height - 30, col_width - 100, 20 };
                    GuiSlider(slider_rect, "0%", "100%", &slider_value, 0.0f, game_state->Power_plants[i].power_max);
                    game_state->Power_plants[i].aimed_power_generation = (int)slider_value;
                    break;
                }
        }
        Texture2D tex = plant_icons[game_state->Power_plants[i].type];

        float icon_size = row_height * 0.4f;

        Rectangle icon_dest = {
            slot.x + (col_width - icon_size) / 2.0f,
            slot.y + (row_height - icon_size) / 2.0f,
            icon_size,
            icon_size
        };

        Color icon_tint = WHITE;
        if (game_state->Power_plants[i].type != 0 && !is_popup_open &&
            CheckCollisionPointRec(GetMousePosition(), icon_dest))
        {
            icon_tint = (Color){200, 200, 255, 200}; // leicht transparent beim Hover
        }

        DrawTexturePro(tex,
            (Rectangle){0, 0, tex.width, tex.height},
            icon_dest, (Vector2){0,0}, 0.0f, icon_tint);

        if (game_state->Power_plants[i].type != 0 && !is_popup_open)
        {
            if (CheckCollisionPointRec(GetMousePosition(), icon_dest) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                popup_just_opened = true;
                is_upgrade_open = true;
                target_plant = i;
            }
        }

        if (!game_state->Power_plants[i].type == 0)
        {
            float percentage;
            if (is_storage(game_state->Power_plants[i].type))
            {
                percentage = (float)game_state->Power_plants[i].capacity / (float)game_state->Power_plants[i].max_capacity;
            }
            else
            {
                percentage = (float)game_state->Power_plants[i].power_generation / (float)game_state->Power_plants[i].power_max;
            }
            if (percentage > 1.0f) percentage = 1.0f;
            if (percentage < 0.0f) percentage = 0.0f;

            int bar_height = 8;
            int bar_y = slot.y + row_height - 45; // über den Buttons

            Rectangle bar_bg = { slot.x + 5, bar_y-3, col_width - 10, bar_height+6 };
            Rectangle bar_fill = { slot.x + 8, bar_y, (col_width - 16) * percentage, bar_height };

            DrawRectangleRec(bar_bg, DARKGRAY);
            if (percentage > 0.8f)
            {
                DrawRectangleRec(bar_fill, GREEN);
            }
            else if (percentage > 0.5f)
            {
                DrawRectangleRec(bar_fill, YELLOW);
            }
            else if (percentage > 0.2f)
            {
                DrawRectangleRec(bar_fill, ORANGE);
            }
            else
            {
                DrawRectangleRec(bar_fill, RED);
            }

        }

    }

}

void render_dashboard(GameState *game_state)
{
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int dashboard_y = screen_h * 0.8f;
    int dashboard_h = screen_h - dashboard_y;

    DrawRectangle(0, dashboard_y, screen_w, dashboard_h, DARKGRAY);

    int font_size = dashboard_h * 0.1f; // skaliert mit Fenstergröße
    int line_spacing = font_size + 8;

    DrawText(TextFormat("Geld: %d", game_state->GridState.money), 20, dashboard_y + 20, font_size, WHITE);
    DrawText(TextFormat("Nutzer: %d", game_state->GridState.power_user_count), 20, dashboard_y + 20 + line_spacing, font_size, WHITE);
    DrawText(TextFormat("Erzeugung: %d kW", game_state->GridState.power_generation), 20, dashboard_y + 20 + line_spacing * 2, font_size, WHITE);
    DrawText(TextFormat("Verbrauch: %d kW", game_state->GridState.power_demand), 20, dashboard_y + 20 + line_spacing * 3, font_size, WHITE);
    DrawText(TextFormat("Zufriedenheit: %d %", game_state->GridState.satisfaction), 20, dashboard_y + 20 + line_spacing * 4, font_size, WHITE);

}

void render_stability_gauge(GameState *game_state)
{
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int center_x = screen_w / 2;
    int center_y = screen_h * 0.985f;
    int radius = screen_h * 0.18f; // skaliert mit Fensterhöhe
    int needle_thickness = screen_h * 0.008f;
    DrawCircleSector((Vector2){center_x, center_y}, radius, 180, 360, 32, RED);
    DrawCircleSector((Vector2){center_x, center_y},radius,200,340,32,ORANGE);
    DrawCircleSector((Vector2){center_x, center_y}, radius, 220, 320, 32, YELLOW);
    DrawCircleSector((Vector2){center_x, center_y}, radius, 240, 300, 32, GREEN);

    float stability_ratio = (float)(game_state->GridState.stability + 100) / 200.0f;
    float angle = 180.0f + stability_ratio * 180.0f;

    Vector2 needle_end = {
        center_x + cosf(angle * DEG2RAD) * radius,
        center_y + sinf(angle * DEG2RAD) * radius
    };

    DrawLineEx((Vector2){center_x, center_y}, needle_end, needle_thickness, BLACK);
}
void render_shop(GameState *game_state)
{       is_popup_open = true;

        int screen_w = GetScreenWidth();
        int screen_h = GetScreenHeight();


        DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

        int panel_w = 400;
        int panel_h = 400;
        int panel_x = (screen_w - panel_w) / 2;
        int panel_y = (screen_h - panel_h) / 2;

        DrawRectangle(panel_x, panel_y, panel_w, panel_h+20, DARKGRAY);
        DrawText("Shop", panel_x + 20, panel_y + 20, 24, WHITE);

        const char *names[76] = {"Solar", "Wind", "Water", "Pump", "Battery", "Coal","Atom"};
        int button_h = 40;

        for (int i = 1; i <= 7; i++)
        {
            Rectangle button = { panel_x + 20, panel_y + 60 + (i-1) * (button_h + 10), panel_w - 40, button_h };

            char label[64];
            sprintf(label, "%s (%d)", names[i-1], plant_defaults[i].cost_per_unit);

            if (GuiButton(button, label) && !popup_just_opened)
            {
                if (simulation_build_plant(game_state, target_plant, i))
                {
                    is_popup_open = false;
                    is_shop_open = false;
                }
            }
        }

        // Close-Button
        Rectangle close_button = { panel_x + panel_w - 100, panel_y+10, 80, 30 };
        if (GuiButton(close_button, "Close"))
        {
            is_popup_open = false;
            is_shop_open = false;
        }


}
void render_power_graph(GameState* game_state)
{


    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int dashboard_y = screen_h * 0.8f;
    int dashboard_h = screen_h - dashboard_y;

    int graph_w = screen_w * 0.3f;
    int graph_h = dashboard_h * 0.7f;

    int graph_x = screen_w - graph_w - 20;
    int graph_y = dashboard_y + (dashboard_h - graph_h) / 2;

    //background rectangle
    DrawRectangle(graph_x, graph_y, graph_w, graph_h, (Color){0, 0, 0, 100});
    DrawRectangleLinesEx((Rectangle){graph_x, graph_y, graph_w, graph_h}, 2, WHITE);

    // find max value
    int max_value = 1;
    for (int i = 0; i < 100; i++)
    {
        if (game_state->GraphData.demand_history[i] > max_value) max_value = game_state->GraphData.demand_history[i];
        if (game_state->GraphData.generation_history[i] > max_value) max_value = game_state->GraphData.generation_history[i];
    }

    //calculate points for graph
    Vector2 demand_points[100];
    Vector2 generation_points[100];

    for (int i = 0; i < 100; i++)
    {
        int idx = (game_state->GraphData.history_index + i) % 100;

        float x = graph_x + (float)i / (100- 1) * graph_w;

        float y_demand = graph_y + graph_h - ((float)game_state->GraphData.demand_history[idx] / max_value) * graph_h;
        float y_gen = graph_y + graph_h - ((float)game_state->GraphData.generation_history[idx] / max_value) * graph_h;

        demand_points[i] = (Vector2){x, y_demand};
        generation_points[i] = (Vector2){x, y_gen};
    }

    //draw lines
    DrawLineStrip(demand_points, 100, RED);
    DrawLineStrip(generation_points, 100, GREEN);

    //information
    DrawText("Demand", graph_x, graph_y - 15, 10, RED);
    DrawText("Generation", graph_x + 60, graph_y - 15, 10, GREEN);


}
void render_day_night_indicator(GameState *game_state)
{
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();


    int center_x = screen_w * 0.27f;
    int center_y = screen_h * 0.985f;
    int radius = screen_h * 0.13f;

    float t = game_state->GridState.time_of_day; // 0.0 - 1.0

    // draw sphere
    DrawCircleSector((Vector2){center_x, center_y}, radius, 180, 360, 32, (Color){30, 30, 60, 255});
    DrawRing((Vector2){center_x, center_y}, radius - 2, radius, 180, 360, 32, DARKGRAY);


    Color sky_color;
    bool is_night = (t < 0.25f || t > 0.75f);

    if (t < 0.25f || t > 0.75f) sky_color = (Color){20, 20, 60, 200};
    else sky_color = (Color){135, 206, 235, 200};

    DrawCircleSector((Vector2){center_x, center_y}, radius, 180, 360, 32, sky_color);

    // calculate position of sun

    float angle = 180.0f + t * 180.0f;

    Vector2 sun_pos = {
        center_x + cosf(angle * DEG2RAD) * radius,
        center_y + sinf(angle * DEG2RAD) * radius
    };

    Color celestial_color = is_night ? LIGHTGRAY : YELLOW;
    DrawCircle(sun_pos.x, sun_pos.y, screen_w * 0.009f, celestial_color);

    //draw time
    float hours_float = t * 24.0f;
    int hours = (int)hours_float;
    int minutes = ((int)((hours_float - hours) * 60.0f) / 10) * 10;

    const char *time_text = TextFormat("%02d:%02d", hours, minutes);
    int text_width = MeasureText(time_text, screen_w * 0.03f);

    DrawText(time_text, center_x - text_width / 2, center_y * 0.95f, screen_w * 0.03f, WHITE);
}

void render_menu(GameState *game_state)
{
    is_popup_open = true;

    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();


    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

    int panel_w = 400;
    int panel_h = 400;
    int panel_x = (screen_w - panel_w) / 2;
    int panel_y = (screen_h - panel_h) / 2;

    DrawRectangle(panel_x, panel_y, panel_w, panel_h, DARKGRAY);
    DrawText("MENU", panel_x + 20, panel_y + 20, 24, WHITE);


        int button_h = 40;


        Rectangle back_to_game_button = { panel_x + 20, panel_y + 60+1 * (button_h + 10), panel_w - 40, button_h };
        if (GuiButton(back_to_game_button, "Back to Game"))
        {
            is_popup_open = false;
            is_menu_open = false;

        }
        Rectangle save_game_button = { panel_x + 20, panel_y + 60+2 * (button_h + 10), panel_w - 40, button_h };
        if (GuiButton(save_game_button, "Save Game"))
        {

            is_menu_open = false;
            popup_just_opened = true;
            is_save_game_open = true;

        }
        Rectangle load_game_button = { panel_x + 20, panel_y + 60+3 * (button_h + 10), panel_w - 40, button_h };
        if (GuiButton(load_game_button, "Load Game"))
        {

            is_menu_open = false;
            popup_just_opened = true;
            is_load_game_open = true;

        }
        Rectangle settings_button = { panel_x + 20, panel_y + 60+4 * (button_h + 10), panel_w - 40, button_h };
        if (GuiButton(settings_button, "Settings"))
        {
            is_menu_open = false;
            popup_just_opened = true;
            is_settings_open = true;
        }
        Rectangle quit_button = { panel_x + 20, panel_y + 60+5 * (button_h + 10), panel_w - 40, button_h };
        if (GuiButton(quit_button, "QUIT"))
        {
          quit_game = true;
        }


}

void render_save_game_popup(GameState* game_state)
{
    is_popup_open = true;
    is_menu_open = false;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();


    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

    int panel_w = 400;
    int panel_h = 250;
    int panel_x = (screen_w - panel_w) / 2;
    int panel_y = (screen_h - panel_h) / 2;

    DrawRectangle(panel_x, panel_y, panel_w, panel_h, DARKGRAY);
    DrawText("SAVE GAME", panel_x + 20, panel_y + 20, 24, WHITE);

    int button_h = 40;
    Rectangle back_to_game_button = { panel_x + 20, panel_y + 180, panel_w - 40, button_h };
    if (GuiButton(back_to_game_button, "Back") && !popup_just_opened)
    {
        is_save_game_open = false;
        is_menu_open = true;
        popup_just_opened = true;

    }

    static char filename_buffer[64] = "game_save_name";
    static bool text_box_edit_mode = false;

    Rectangle textbox_rect = { panel_x+20, panel_y+100, 260, 30 };
    Rectangle save_button_rect = { panel_x + 290, panel_y+100, 80, 30 };

    if (GuiTextBox(textbox_rect, filename_buffer, 64, text_box_edit_mode))
    {
        text_box_edit_mode = !text_box_edit_mode;
    }

    if (GuiButton(save_button_rect, "Save") && !popup_just_opened)
    {
        char full_path[80];
        sprintf(full_path, "%s.dat", filename_buffer); // z.B. "savegame.dat"
        if (save_game(game_state, full_path))
        {
            is_save_game_open = false;
            is_menu_open = true;
            popup_just_opened = true;
        }

    }


}

void render_load_game_popup(GameState* game_state)
{
    is_popup_open = true;
    is_menu_open = false;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();


    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

    int panel_w = 400;
    int panel_h = 250;
    int panel_x = (screen_w - panel_w) / 2;
    int panel_y = (screen_h - panel_h) / 2;

    DrawRectangle(panel_x, panel_y, panel_w, panel_h, DARKGRAY);
    DrawText("LOAD GAME", panel_x + 20, panel_y + 20, 24, WHITE);

    int button_h = 40;
    Rectangle back_to_game_button = { panel_x + 20, panel_y + 180, panel_w - 40, button_h };
    if (GuiButton(back_to_game_button, "Back") && !popup_just_opened)
    {
        if (game_state->is_playing)
        {
            is_load_game_open = false;
            is_menu_open = true;
            popup_just_opened = true;
        }
        else
        {
            is_load_game_open = false;
            is_menu_open = false;
            is_popup_open = false;
        }

    }

    static char filename_buffer[64] = "game_save_name";
    static bool text_box_edit_mode = false;

    Rectangle textbox_rect = { panel_x+20, panel_y+100, 260, 30 };
    Rectangle load_button_rect = { panel_x + 290, panel_y+100, 80, 30 };

    if (GuiTextBox(textbox_rect, filename_buffer, 64, text_box_edit_mode))
    {
        text_box_edit_mode = !text_box_edit_mode;
    }

    if (GuiButton(load_button_rect, "Load") && !popup_just_opened)
    {
        char full_path[80];
        sprintf(full_path, "%s.dat", filename_buffer);
        if (load_game(game_state, full_path))
        {
            if (game_state->is_playing)
            {
                is_load_game_open = false;
                is_menu_open = true;
                popup_just_opened = true;
            }
            else
            {
                is_load_game_open = false;
                game_state->is_playing = true;
                is_popup_open = false;
            }
        }
    }


}

void render_settings(GameState* game_state)
{
    is_popup_open = true;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();


    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

    int panel_w = 600;
    int panel_h = 600;
    int panel_x = (screen_w - panel_w) / 2;
    int panel_y = (screen_h - panel_h) / 2;

    DrawRectangle(panel_x, panel_y, panel_w, panel_h, DARKGRAY);
    DrawText("SETTINGS", panel_x + 20, panel_y + 20, 24, WHITE);

    Rectangle close_button = { panel_x + panel_w - 100, panel_y+10, 80, 30 };
    if (GuiButton(close_button, "Close"))
    {
        if (game_state->is_playing)
        {
            is_settings_open = false;
            is_menu_open = true;
            popup_just_opened = true;
        }
        else
        {
            is_settings_open = false;
            is_popup_open = false;
        }


    }
    int button_h = 40;
            Rectangle toggle_fullscreen_button = { panel_x + 20, panel_y + 100, panel_w - 40, button_h };
            if (GuiButton(toggle_fullscreen_button, "Toggle Fullscreen") && !popup_just_opened)
            {
                ToggleFullscreen();
            }







}
void render_title_screen(GameState* game_state, float time)
{
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    // Sanfter Farbverlauf, der sich langsam über Zeit verschiebt
    float shift = sinf(time * 0.1f) * 0.5f + 0.5f; // 0.0 - 1.0, langsam oszillierend

    Color top = ColorLerp((Color){20, 30, 60, 255}, (Color){40, 50, 90, 255}, shift);
    Color bottom = (Color){10, 15, 30, 255};

    DrawRectangleGradientV(0, 0, screen_w, screen_h, top, bottom);

    const char *title = "POWER GRID";
    int font_size = screen_w * 0.06f;
    int text_width = MeasureText(title, font_size);

    int x = screen_w / 2 - text_width / 2;
    int y = screen_h * 0.25f;

    // Schatten (leicht versetzt, dunkler)
    DrawText(title, x + 4, y + 4, font_size, (Color){0, 0, 0, 150});
    // Haupttext
    DrawText(title, x, y, font_size, WHITE);

    const char *subtitle = "GAME";
    int sub_size = font_size * 0.4f;
    int sub_width = MeasureText(subtitle, sub_size);
    DrawText(subtitle, screen_w / 2 - sub_width / 2, y + font_size + 5, sub_size, (Color){200, 200, 200, 255});

    int button_w = screen_w * 0.25f;
    int button_h = 50;
    int center_x = screen_w / 2 - button_w / 2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    Rectangle play_button = { center_x, screen_h * 0.55f, button_w, button_h };
    if (GuiButton(play_button, "PLAY")&& !is_popup_open)
    {
        game_state->is_playing = true;
    }

    Rectangle load_button = { center_x, screen_h * 0.55f + 60, button_w, button_h };
    if (GuiButton(load_button, "LOAD GAME")&& !is_popup_open)
    {
        popup_just_opened = true;
        is_load_game_open = true;
    }
    Rectangle settings_button = { center_x, screen_h * 0.55f + 120, button_w, button_h };
    if (GuiButton(settings_button, "SETTINGS")&& !is_popup_open)
    {
        popup_just_opened = true;
        is_settings_open = true;
    }
    Rectangle quit_button = { center_x, screen_h * 0.55f + 180, button_w, button_h };
    if (GuiButton(quit_button, "QUIT")&& !is_popup_open)
    {
        quit_game = true;
    }
}

void render_upgrade_popup(GameState* game_state)
{
    is_popup_open = true;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

    int panel_w = 400;
    int panel_h = 300;
    int panel_x = (screen_w - panel_w) / 2;
    int panel_y = (screen_h - panel_h) / 2;

    DrawRectangle(panel_x, panel_y, panel_w, panel_h, DARKGRAY);

    PowerPlant_t *p = &game_state->Power_plants[target_plant];

    DrawText(TextFormat("Plant Level: %d", p->upgrade_level), panel_x + 20, panel_y + 20, 20, WHITE);

    DrawText(TextFormat("Max Pow.: %d kW / Lvl.%d: %dkW", p->power_max,p->upgrade_level+1,p->power_max+p->power_boost), panel_x + 20, panel_y + 60, 20, WHITE);
    if (p->max_capacity > 0)
    {
        DrawText(TextFormat("Max Cap.: %d kWh / Lvl.%d: %dkWh", p->max_capacity,p->upgrade_level+1,p->max_capacity+p->capacity_boost), panel_x + 20, panel_y + 80, 20, WHITE);
    }

    int button_h = 40;

    Rectangle upgrade_button = { panel_x + 20, panel_y + 120, panel_w - 40, button_h };
    if (GuiButton(upgrade_button, TextFormat("Upgrade (%d)", p->upgrade_cost)) && !popup_just_opened)
    {


        if (game_state->GridState.money >= p->upgrade_cost)
        {
            game_state->GridState.money -= p->upgrade_cost;
            p->power_max += p->power_boost;
            p->max_capacity += p->capacity_boost;
            p->upgrade_level++;
            is_upgrade_open = false;
            is_popup_open = false;
        }
    }

    Rectangle sell_button = { panel_x + 20, panel_y + 170, panel_w - 40, button_h };
    int sell_value = (p->cost_per_unit+p->upgrade_level * p->upgrade_cost) * 0.8f;
    if (GuiButton(sell_button, TextFormat("Sell (%d)", sell_value)) && !popup_just_opened)
    {

        game_state->GridState.money += sell_value;
        *p = plant_defaults[0];
        is_upgrade_open = false;
        is_popup_open = false;
    }

    Rectangle close_button = { panel_x + 20, panel_y + 230, panel_w - 40, button_h };
    if (GuiButton(close_button, "Close"))
    {
        is_upgrade_open = false;
        is_popup_open = false;
    }
}

