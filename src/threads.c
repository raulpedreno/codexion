#include "../include/codexion.h"

int	start_threads(t_sim *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (1);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	join_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		return (1);
	return (0);
}