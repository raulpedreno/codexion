#include "../include/codexion.h"

int main(int argc, char **argv)
{
    t_sim    sim;
    t_config config;

    if (parse_args(argc, argv, &config) != 0)
        return (1);
    if (init_sim(&sim, config) != 0)
    {
        cleanup_sim(&sim);
        return (1);
    }
    if (start_threads(&sim) != 0)
    {
        cleanup_sim(&sim);
        return (1);
    }
    if (join_threads(&sim) != 0)
    {
        cleanup_sim(&sim);
        return (1);
    }
    cleanup_sim(&sim);
    return (0);
}