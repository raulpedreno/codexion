#include "../include/codexion.h"

int	is_sim_active(t_sim *sim)
{
	int	active;

	pthread_mutex_lock(&sim->active_mutex);
	active = sim->active;
	pthread_mutex_unlock(&sim->active_mutex);
	return (active);
}

void	stop_sim(t_sim *sim)
{
	pthread_mutex_lock(&sim->active_mutex);
	sim->active = 0;
	pthread_mutex_unlock(&sim->active_mutex);
}