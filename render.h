#ifndef POWER_GAME_RENDER_H
#define POWER_GAME_RENDER_H
#include "raylib.h"
#include "simulation.h"

extern bool is_popup_open;
extern bool popup_just_opened;
extern bool is_shop_open;
extern bool is_menu_open;
extern bool is_settings_open;
extern bool is_save_game_open;
extern bool is_load_game_open;
extern bool quit_game;


void load_textures(void);
void render_top_zone(GameState* game_state);
void render_dashboard(GameState* game_state);
void render_stability_gauge(GameState* game_state);
void render_shop(GameState *game_state);
void render_power_graph(GameState* game_state);
void render_day_night_indicator(GameState* game_state);
void render_menu(GameState* game_state);
void render_save_game_popup(GameState* game_state);
void render_load_game_popup(GameState* game_state);
void render_settings(GameState* game_state);
void render_title_screen(GameState* game_state, float time);







#endif //POWER_GAME_RENDER_H
