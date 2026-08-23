#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include <stdio.h>
#include "plant.h"

bool is_shop_open = false;
int shop_target_plant = 0;
bool shop_just_opened = false;

Texture2D plant_icons[8];

void load_textures(void)
{
    plant_icons[0] = LoadTexture("Pictures/no_power_plant.png");
    plant_icons[1] = LoadTexture("Pictures/solar.png");
    plant_icons[2] = LoadTexture("Pictures/wind.png");
    plant_icons[3] = LoadTexture("Pictures/water_power_plant.png");
    plant_icons[4] = LoadTexture("Pictures/pump_power_plant.png");
    plant_icons[5] = LoadTexture("Pictures/battery.png");
    plant_icons[6] = LoadTexture("Pictures/coal_power_plant.png");
    plant_icons[7] = LoadTexture("Pictures/nuclear_power_plant.png");
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
        if (game_state->Power_plants[i].status == 1) slot_color = GREEN;
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
                    if (GuiButton(buy_plant_button, "Buy Plant"))
                    {
                        is_shop_open = true;
                        shop_just_opened = true;
                        shop_target_plant = i;
                    }
                    break;
                }
            case 1:
                {
                    DrawText(TextFormat("Solar"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
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
                    if (GuiButton(start_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
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
                    if (GuiButton(start_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    break;
                }
            case 4:
                {
                    DrawText(TextFormat("Pump"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %d kW | %d kWh / %d kWh", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max,game_state->Power_plants[i].capacity,game_state->Power_plants[i].max_capacity),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(start_plant_button, "Pump"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/4, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    Rectangle regenerate_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(regenerate_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 2;
                    }
                    Rectangle auto_plant_button = { slot.x + ((col_width+2)/4)*3, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(auto_plant_button, "Auto"))
                    {
                        game_state->Power_plants[i].status = 3;
                    }
                    break;
                }
            case 5:
                {
                    DrawText(TextFormat("Battery"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %d kW | %d kWh / %d kWh", game_state->Power_plants[i].power_generation, game_state->Power_plants[i].power_max,game_state->Power_plants[i].capacity,game_state->Power_plants[i].max_capacity),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(start_plant_button, "Pump"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/4, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    Rectangle regenerate_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(regenerate_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 2;
                    }
                    Rectangle auto_plant_button = { slot.x + ((col_width+2)/4)*3, slot.y + row_height - 30, (col_width-4) / 4, 30 };
                    if (GuiButton(auto_plant_button, "Auto"))
                    {
                        game_state->Power_plants[i].status = 3;
                    }
                    break;
                }
            case 6:
                {
                    DrawText(TextFormat("Coal"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, min(game_state->Power_plants[i].power_max,game_state->Power_plants[i].aimed_power_generation)),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
                    {
                        game_state->Power_plants[i].status = 0;
                    }
                    break;
                }
            case 7:
                {
                    DrawText(TextFormat("Atom Plant"), slot.x + 5, slot.y + 5, 10, BLACK);
                    DrawText(TextFormat("%d kW / %dkW", game_state->Power_plants[i].power_generation, min(game_state->Power_plants[i].power_max,game_state->Power_plants[i].aimed_power_generation)),slot.x + 5, slot.y + 25, plant_info_text_size, BLACK);
                    Rectangle start_plant_button = { slot.x+2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(start_plant_button, "Start"))
                    {
                        game_state->Power_plants[i].status = 1;
                    }
                    Rectangle stop_plant_button = { slot.x + (col_width+2)/2, slot.y + row_height - 30, (col_width-4) / 2, 30 };
                    if (GuiButton(stop_plant_button, "Stop"))
                    {
                        game_state->Power_plants[i].status = 0;
                    }
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

        DrawTexturePro(tex,
            (Rectangle){0, 0, tex.width, tex.height},
            icon_dest, (Vector2){0,0}, 0.0f, WHITE);

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
    DrawText(TextFormat("NetzStabilität: %d", game_state->GridState.stability), 20, dashboard_y + 20 + line_spacing * 5, font_size, WHITE);
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
{       is_shop_open = true;

        int screen_w = GetScreenWidth();
        int screen_h = GetScreenHeight();

        // Halbtransparentes Overlay über allem
        DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});

        int panel_w = screen_w * 0.5f;
        int panel_h = screen_h * 0.6f;
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

            if (GuiButton(button, label) && !shop_just_opened)
            {
                if (simulation_build_plant(game_state, shop_target_plant, i))
                {
                    is_shop_open = false;
                }
            }
        }

        // Close-Button
        Rectangle close_button = { panel_x + panel_w - 100, panel_y+10, 80, 30 };
        if (GuiButton(close_button, "Close"))
        {
            is_shop_open = false;
        }
        shop_just_opened = false;

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
void render_day_info(GameState* game_state)
{
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();
}
