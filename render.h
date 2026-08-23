#ifndef POWER_GAME_RENDER_H
#define POWER_GAME_RENDER_H
#include "simulation.h"

extern bool is_shop_open;

void load_textures(void);
void render_top_zone(GameState* game_state);
void render_dashboard(GameState* game_state);
void render_stability_gauge(GameState* game_state);
void render_shop(GameState *game_state);
void render_power_graph(GameState* game_state);
void render_day_info(GameState* game_state);








#endif //POWER_GAME_RENDER_H
