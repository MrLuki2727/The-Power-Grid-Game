#ifndef POWER_GAME_SIMULATION_H
#define POWER_GAME_SIMULATION_H

typedef struct
{
    int type; // Power Plant type: 0 = no plant; 1 = Solar; 2 = Wind_turbine; 3 = Water; 4 = pump; 5 = battery 6 = Coal; 7 = Atom Plant;
    int power_generation; // Power generation now
    int power_max; // Power generation max
    int power_cost; // Power cost
    int reaction_time; // Reaction time
    int status; // 0 = Off; 1 = On / Charging; 2 = Discharging; 3 = auto
    int capacity; //  Current capacity for energy storing of the power plant
    int max_capacity; // Max energy storing capacity of the power plant
    int cost_per_unit; // Cost per unit
    int max_power_deviation; // How much the power production can very max
    int power_deviation_per_tick; // How much the power production can very every tick
    int emission_factor; //How much emission the plant produces per tick;
    int aimed_power_generation; // Power generation that the plant is aiming for (-1 = no regulation)

    int upgrade_level;
    int upgrade_cost;
    int capacity_boost;
    int power_boost;
}PowerPlant_t;

typedef struct
{
    int power_demand; //Power consumption of all users combined
    int power_generation; // Power generation of all power plants combined
    int power_cost; // Power cost of all power plants combined
    int total_emissions; //Emissions per power unit
    int stability; // Stability of the system (-100) - (100)
    int money; // Money of the user
    int power_user_count; // Number of the power users
    int satisfaction; // 0 - 100
    float time_of_day;
    float day_length_seconds;
    float user_growth_timer;
    float deviation_timer;
    float income_timer;


}GridState_t;

typedef struct
{
    int demand_history[100];
    int generation_history[100];
    int history_index; // Index of the last entry in the history when 100 then move the whole array and delete the last
}GraphData_t;

typedef struct
{
    PowerPlant_t Power_plants[8]; // 8 Power plants
    GridState_t GridState;
    GraphData_t GraphData;
    bool is_playing;
}GameState;





void simulation_update(GameState *game_state, float delta_time);// Game goes one step further updating power ganeration / usage

bool simulation_build_plant(GameState *game_state, int slot, int type); // Slot 0 - 7; type: 0 = no plant; 1 = Solar; 2 = Wind_turbine; 3 = Water; 4 = pump; 5 = battery; 6 = Coal; 7 = Atom Plant;

void simulation_init(GameState *game_state);

bool is_storage(int type);
bool is_regulatable(int type);
bool save_game(GameState *game_state, const char *filename);
bool load_game(GameState *game_state, const char *filename);

#endif //POWER_GAME_SIMULATION_H
