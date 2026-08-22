#include "plant.h"
#include <math.h>

const PowerPlant_t plant_defaults[8] = {
    // type, power_generation, power_max, power_cost, reaction_time, is_active, capacity, max_capacity, cost_per_unit, max_power_deviation, power_deviation_per_tick, emission_factor, aimed_power_generation
    {0, 0, 0,   0,  0, 0, 0,   0,   0,   0, 0,  0,  0},   // NONE
    {1, 0, 50,  20, 1, 0, 0,   0,   100, 25, 10, 0,  -1},   // SOLAR
    {2, 0, 80,  25, 2, 0, 0,   0,   150, 25,  5,0,  -1},   // WIND
    {3, 0, 120, 30, 1, 0, 0,   0,   300, 5,   2,0,  -1},   // WATER
    {4, 0, 60,  10, 3, 0, 0,   1000, 250, 5, 2, 0,  -1},   // PUMP
    {5, 0, 40,  5,  1, 0, 0,   200, 180, 5,   1,0,  -1},   // BATTERY
    {6, 0, 150, 40, 4, 0, 0,   0,   400, 5,   1,80, 0},   // COAL
    {7, 0, 300, 60, 8, 0, 0,   0,   800, 2,  1,5,  0},   // NUCLEAR
};


float solar_output_factor(float time_of_day)
{
    float sunrise = 0.25f;
    float sunset = 0.75f;

    if (time_of_day < sunrise || time_of_day > sunset)
        return 0.0f;

    float day_progress = (time_of_day - sunrise) / (sunset - sunrise);
    return sinf(day_progress * M_PI);
}
float demand_curve_factor(float time_of_day)
{
    float morning_peak = expf(-powf((time_of_day - 0.33f) * 12.0f, 2));
    float evening_peak  = expf(-powf((time_of_day - 0.8f) * 10.0f, 2));

    float base_load = 0.4f;
    return base_load + morning_peak * 0.4f + evening_peak * 0.6f;
}