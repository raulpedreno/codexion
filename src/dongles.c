#include "../include/codexion.h"

static int	get_left_dongle(t_coder *coder)
{
	return (coder->id - 1);
}

static int	get_right_dongle(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	return (coder->id % sim->config.number_of_coders);
}

void	take_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;

	sim = coder->sim;
	left = get_left_dongle(coder);
	right = get_right_dongle(coder);
	if (sim->config.number_of_coders == 1)
	{
		pthread_mutex_lock(&sim->dongles[left].mutex);
		print_log(sim, coder->id, "has taken a dongle");
		while (is_sim_active(sim))
			smart_sleep(sim, 1);
		pthread_mutex_unlock(&sim->dongles[left].mutex);
		return ;
	}
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&sim->dongles[right].mutex);
		print_log(sim, coder->id, "has taken a dongle");
		pthread_mutex_lock(&sim->dongles[left].mutex);
		print_log(sim, coder->id, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&sim->dongles[left].mutex);
		print_log(sim, coder->id, "has taken a dongle");
		pthread_mutex_lock(&sim->dongles[right].mutex);
		print_log(sim, coder->id, "has taken a dongle");
	}
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;

	sim = coder->sim;
	left = get_left_dongle(coder);
	right = get_right_dongle(coder);
	pthread_mutex_unlock(&sim->dongles[right].mutex);
	pthread_mutex_unlock(&sim->dongles[left].mutex);
}