#include "../include/codexion.h"

int main(int argc, char **argv)
{
    t_sim sim;
    t_config config;

    // 1) parse_args → rellenas config
    if (parse_args(argc, argv, &config) != 0)
        return (1);

    // 2) init_sim → construyes el mundo
    if (init_sim(&sim, config) != 0)
        return (1);

    // 3) start_threads → creas los hilos (coders)
    if (start_threads(&sim) != 0)
        return (1);

    // 4) join_threads → esperas a que todos terminen
    if (join_threads(&sim) != 0)
        return (1);

    // 5) cleanup_sim → destruyes todo
    cleanup_sim(&sim);
    return (0);
}

