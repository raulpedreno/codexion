#include "../include/codexion.h"

static void	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].burned_out = 0;
		sim->coders[i].last_compile_start_ms = sim->start_ms;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
			return (1);
		i++;
	}
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].in_use = 0;
		sim->dongles[i].released_at_ms = 0;
		sim->dongles[i].queue.data = NULL;
		sim->dongles[i].queue.size = 0;
		sim->dongles[i].queue.capacity = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_sim(t_sim *sim, t_config config)
{
	sim->config = config;
	sim->active = 1;
	sim->start_ms = get_time_ms();
	sim->coders = malloc(sizeof(t_coder) * config.number_of_coders);
	if (!sim->coders)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
	if (!sim->dongles)
		return (1);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->active_mutex, NULL) != 0)
		return (1);
	if (init_coders(sim) != 0)
		return (1);
	if (init_dongles(sim) != 0)
		return (1);
	return (0);
}