#ifndef POWER_GAME_PLANT_H
#define POWER_GAME_PLANT_H
#include "simulation.h"
extern const PowerPlant_t plant_defaults[8];
float solar_output_factor(float time_of_day);
float demand_curve_factor(float time_of_day);

#endif //POWER_GAME_PLANT_H
