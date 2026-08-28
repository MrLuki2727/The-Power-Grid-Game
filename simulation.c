#include <stdlib.h>
#include "simulation.h"

#include <stdio.h>

#include "plant.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define DEVIATION_INTERVAL 3.0f     // seconds between random weather noise ticks
#define USER_GROWTH_INTERVAL 10.0f  // seconds between user count changes
#define INCOME_INTERVAL 5.0f

bool is_storage(int type)
{
    return type == 4 || type == 5; // 4 = Pump, 5 = Battery
}

bool is_regulatable(int type)
{
    return type == 6 || type == 7;
}

static float rand_deviation(void)
{
    return ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
}

bool simulation_build_plant(GameState *game_state, int slot, int type)
{
    if (slot < 0 || slot > 7) return false;
    if (game_state->Power_plants[slot].type != 0) return false;
    if (type < 1 || type > 7) return false;

    int build_cost = plant_defaults[type].cost_per_unit;
    if (game_state->GridState.money < build_cost) return false;

    game_state->Power_plants[slot] = plant_defaults[type];
    game_state->GridState.money -= build_cost;

    return true;
}

// Updates non-storage plants (solar, wind, water, coal, nuclear).
// Storage (pump/battery) is handled separately in simulation_update.
static void update_plant(PowerPlant_t *plant, float time_of_day, bool do_deviation_tick)
{
    // Off or empty slot: ramp down to 0 just like everything else, not an instant cut
    int target;
    float solar_factor = 0.0f;

    if (is_regulatable(plant->type)) {
        target = plant->aimed_power_generation;
    }
    else if (plant->type == 0 || plant->status == 0)
    {
        target = 0;
    }
    else if (is_storage(plant->type))
    {
        return; // handled separately in simulation_update
    }
    else if (plant->type == 1) // Solar: target follows day/night curve
    {
        solar_factor = solar_output_factor(time_of_day);
        target = (int)(plant->power_max * solar_factor);
    }
    else if (plant->aimed_power_generation < 0) // Wind/Water: always aim for max
    {
        target = plant->power_max;
    }




    int max_step = plant->power_max / plant->reaction_time;
    if (max_step < 1) max_step = 1;

    // Ramp power_generation towards target in both directions, limited by max_step
    int diff = target - plant->power_generation;
    if (diff > max_step) diff = max_step;
    else if (diff < -max_step) diff = -max_step;
    plant->power_generation += diff;

    // Random weather noise, only every few seconds, and never pushes solar above 0 at night
    if (plant->type != 0 && plant->status != 0 && plant->max_power_deviation > 0 && do_deviation_tick)
    {
        bool solar_is_night = (plant->type == 1 && solar_factor <= 0.0f);

        if (!solar_is_night)
        {
            float deviation = rand_deviation() * plant->power_deviation_per_tick;
            int offset = plant->power_generation - target;

            if (offset >= plant->max_power_deviation && deviation > 0) deviation = -deviation;
            else if (offset <= -plant->max_power_deviation && deviation < 0) deviation = -deviation;

            plant->power_generation += (int)deviation;
        }
    }

    if (plant->power_generation < 0) plant->power_generation = 0;
    if (plant->power_generation > plant->power_max) plant->power_generation = plant->power_max;
}

// Satisfaction drops when: grid unstable, electricity expensive, or emissions high
static int calculate_satisfaction(GameState *game_state)
{
    int satisfaction = 100;

    int stability_abs = abs(game_state->GridState.stability);
    satisfaction -= stability_abs;

    int generation_safe = max(1, game_state->GridState.power_generation);
    int price_per_unit = game_state->GridState.power_cost / generation_safe;
    satisfaction -= price_per_unit / 50;

    satisfaction -= game_state->GridState.total_emissions / 200;

    if (satisfaction > 100) satisfaction = 100;
    if (satisfaction < 0) satisfaction = 0;

    return satisfaction;
}

void simulation_update(GameState *game_state, float delta_time)
{
    // 1. Advance time of day (0.0 - 1.0, wraps around)
    game_state->GridState.time_of_day += delta_time / game_state->GridState.day_length_seconds;
    if (game_state->GridState.time_of_day >= 1.0f) game_state->GridState.time_of_day -= 1.0f;

    // 2. Deviation timer: random noise only happens every DEVIATION_INTERVAL seconds
    game_state->GridState.deviation_timer += delta_time;
    bool do_deviation_tick = false;
    if (game_state->GridState.deviation_timer >= DEVIATION_INTERVAL)
    {
        do_deviation_tick = true;
        game_state->GridState.deviation_timer -= DEVIATION_INTERVAL;
    }

    int total_generation = 0;
    int total_cost = 0;
    int total_emissions = 0;

    // 3. Update all non-storage plants
    for (int i = 0; i < 8; i++)
    {
        PowerPlant_t *p = &game_state->Power_plants[i];
        if (!is_storage(p->type))
        {
            update_plant(p, game_state->GridState.time_of_day, do_deviation_tick);
            total_generation += p->power_generation;
            total_cost += p->power_generation * p->cost_per_unit;
            total_emissions += p->power_generation * p->emission_factor;
        }
    }

    // 4. Demand from user count + time-of-day curve
    float demand_factor = demand_curve_factor(game_state->GridState.time_of_day);
    int base_demand_per_user = 5;
    game_state->GridState.power_demand = (int)(game_state->GridState.power_user_count * base_demand_per_user * demand_factor);

    int surplus = total_generation - game_state->GridState.power_demand;

    // 5. Collect total charge/discharge capacity of all AUTO-mode storage plants
    //    (used to split surplus/deficit fairly between multiple auto storages)
    int total_charge_capacity = 0;
    int total_discharge_capacity = 0;

    for (int i = 0; i < 8; i++)
    {
        PowerPlant_t *p = &game_state->Power_plants[i];
        if (is_storage(p->type) && p->status == 3)
        {
            total_charge_capacity += (p->max_capacity - p->capacity);
            total_discharge_capacity += p->capacity;
        }
    }

    // 6. Apply storage actions (charge/discharge), ramped by max_step in both directions
    for (int i = 0; i < 8; i++)
    {
        PowerPlant_t *p = &game_state->Power_plants[i];
        if (!is_storage(p->type)) continue;

        int max_step = p->power_max / p->reaction_time;
        if (max_step < 1) max_step = 1;

        // desired_action: positive = discharge (feed grid), negative = charge (draw from grid), 0 = idle/off
        int desired_action = 0;

        if (p->status == 1) // manual charge - only if grid actually has surplus (no free energy!)
        {
            if (surplus > 0)
                desired_action = -min(surplus, p->max_capacity - p->capacity);
        }
        else if (p->status == 2) // manual discharge
        {
            desired_action = min(p->power_max, p->capacity);
        }
        else if (p->status == 3) // auto: split surplus/deficit proportionally between auto storages
        {
            if (surplus > 0 && total_charge_capacity > 0)
            {
                int can_charge = p->max_capacity - p->capacity;
                int share = (surplus * can_charge) / total_charge_capacity;
                desired_action = -min(share, can_charge);
            }
            else if (surplus < 0 && total_discharge_capacity > 0)
            {
                int can_discharge = p->capacity;
                int share = (-surplus * can_discharge) / total_discharge_capacity;
                desired_action = min(share, can_discharge);
            }
        }
        // status == 0: desired_action stays 0 -> ramps power_generation down to 0, not instant off

        // Ramp power_generation towards desired_action, limited by max_step, in both directions
        int diff = desired_action - p->power_generation;
        if (diff > max_step) diff = max_step;
        else if (diff < -max_step) diff = -max_step;
        p->power_generation += diff;

        if (p->power_generation > p->power_max) p->power_generation = p->power_max;
        if (p->power_generation < -p->power_max) p->power_generation = -p->power_max;

        p->capacity -= p->power_generation;
        if (p->capacity < 0) p->capacity = 0;
        if (p->capacity > p->max_capacity) p->capacity = p->max_capacity;

        total_generation += p->power_generation;

        // Only discharging counts as "cost" (charging is just moving energy around, not buying more)
        if (p->power_generation > 0)
        {
            total_cost += p->power_generation * p->cost_per_unit;
        }
    }

    game_state->GridState.power_generation = total_generation;
    game_state->GridState.power_cost = total_cost;
    game_state->GridState.total_emissions = total_emissions;

    // 7. Stability as percentage deviation from demand (-100 to 100), not raw kW
    int diff = total_generation - game_state->GridState.power_demand;
    int demand_safe = max(1, game_state->GridState.power_demand);
    float percent_diff = ((float)diff / (float)demand_safe) * 100.0f;

    if (percent_diff > 100.0f) percent_diff = 100.0f;
    if (percent_diff < -100.0f) percent_diff = -100.0f;
    game_state->GridState.stability = (int)percent_diff;

    // No users = no demand = treat grid as neutral, not unstable
    if (game_state->GridState.power_user_count == 0)
    {
        game_state->GridState.stability = 0;
    }

    game_state->GridState.satisfaction = calculate_satisfaction(game_state);

    // calculate income
    game_state->GridState.income_timer += delta_time;

    if (game_state->GridState.income_timer >= INCOME_INTERVAL)
    {
        game_state->GridState.income_timer = 0.0f;

        // Stability bonus: 1.0 at perfect stability, 0.0 at the edges (+-100)
        int stability_abs = abs(game_state->GridState.stability);
        float stability_bonus = 1.0f - (float)stability_abs / 100.0f;
        if (stability_bonus < 0) stability_bonus = 0;

        // Revenue based on how much power was actually delivered to users, not just user count
        float price_per_kw = 0.2f; // tune this to balance income vs plant costs (100-800)

        int income = (int)(game_state->GridState.power_demand * price_per_kw * stability_bonus);

        game_state->GridState.money += income;

        if (game_state->GridState.money < 0) game_state->GridState.money = 0;
    }

    // 8. User count changes slowly (every USER_GROWTH_INTERVAL seconds), NOT every tick
    game_state->GridState.user_growth_timer += delta_time;

    if (game_state->GridState.user_growth_timer >= USER_GROWTH_INTERVAL)
    {
        game_state->GridState.user_growth_timer = 0.0f;

        if (game_state->GridState.satisfaction > 70)
        {
            game_state->GridState.power_user_count += 1;
        }
        else if (game_state->GridState.satisfaction < 30)
        {
            game_state->GridState.power_user_count -= 1;
            if (game_state->GridState.power_user_count < 0) game_state->GridState.power_user_count = 0;
        }
    }

    // 9. Update graph history (shifting ring buffer)
    game_state->GraphData.demand_history[game_state->GraphData.history_index] = game_state->GridState.power_demand;
    game_state->GraphData.generation_history[game_state->GraphData.history_index] = game_state->GridState.power_generation;

    if (game_state->GraphData.history_index < 99)
    {
        game_state->GraphData.history_index++;
    }
    else
    {
        for (int i = 0; i < 99; i++)
        {
            game_state->GraphData.demand_history[i] = game_state->GraphData.demand_history[i + 1];
            game_state->GraphData.generation_history[i] = game_state->GraphData.generation_history[i + 1];
        }
    }
#define BLACKOUT_THRESHOLD 80
#define BLACKOUT_DURATION 5.0f



    int stability_abs = abs(game_state->GridState.stability);

    if (stability_abs >= BLACKOUT_THRESHOLD)
    {
        game_state->GridState.blackout_timer += delta_time;
    }
    else
    {
        game_state->GridState.blackout_timer -= delta_time * 2.0f; // schneller abbauen als aufbauen
        if (game_state->GridState.blackout_timer < 0) game_state->GridState.blackout_timer = 0;
    }

    if (game_state->GridState.blackout_timer >= BLACKOUT_DURATION)
    {
        game_state->GridState.game_over = true;
    }
}

void simulation_init(GameState *game_state)
{
    game_state->GridState.money = 100;
    game_state->GridState.power_user_count = 10;
    game_state->GridState.stability = 0;
    game_state->GridState.satisfaction = 50;
    game_state->GridState.time_of_day = 0.5f;
    game_state->GridState.day_length_seconds = 300.0f;
    game_state->GridState.user_growth_timer = 0.0f;
    game_state->GridState.deviation_timer = 0.0f;
    game_state->is_playing = false;
    game_state->GraphData.history_index = 0;
    game_state->GridState.game_over = false;
    game_state->GridState.blackout_timer = 0.0f;

    game_state->Power_plants[0] = plant_defaults[1]; // Solar
    game_state->Power_plants[1] = plant_defaults[4]; // Pump

    for (int i = 2; i < 8; i++)
    {
        game_state->Power_plants[i] = plant_defaults[0]; // empty slots
    }

    for (int i = 0; i < 100; i++)
    {
        game_state->GraphData.demand_history[i] = 0;
        game_state->GraphData.generation_history[i] = 0;
    }
}
bool save_game(GameState *game_state, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return false;

    int version = 1;
    fwrite(&version, sizeof(int), 1, file);
    fwrite(game_state, sizeof(GameState), 1, file);
    fclose(file);

    return true;
}

bool load_game(GameState *game_state, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return false;

    int version;
    fread(&version, sizeof(int), 1, file);

    if (version != 1) // Struct-Version stimmt nicht -> alter Save, ignorieren
    {
        fclose(file);
        return false;
    }

    fread(game_state, sizeof(GameState), 1, file);
    fclose(file);

    return true;
}