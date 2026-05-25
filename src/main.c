#include "../include/codexion.h"

int main(int argc, char **argv)
{
    t_config config;

    if (parse_args(argc, argv, &config) != 0)
    {
        printf("Error\n");
        return (1);
    }
    // probando config
    printf("number_of_coders: %i\n", config.number_of_coders);
    printf("time_to_burnout: %li\n", config.time_to_burnout);
    printf("time_to_compile: %li\n", config.time_to_compile);
    printf("time_to_debug: %li\n", config.time_to_debug);
    printf("time_to_refactor: %li\n", config.time_to_refactor);
    printf("number_of_compiles_required: %i\n", config.number_of_compiles_required);
    printf("dongle_cooldown: %li\n", config.dongle_cooldown);
    printf("scheduler: %i\n", config.scheduler);

    // probando funcion get_time_ms()
    printf("current time: %li\n", get_time_ms());

    // probando init_sim
    t_sim	sim;

    if (init_sim(&sim, config) != 0)
    {
	    printf("Error\n");
	    return (1);
    }
    printf("sim initialized\n");
    
    return (0);
}
