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

static void	wait_dongle_cooldown(t_sim *sim, t_dongle *dongle)
{
	long	now;
	long	available_at;

	now = get_time_ms();
	available_at = dongle->released_at_ms + sim->config.dongle_cooldown;
	if (now < available_at)
		smart_sleep(sim, available_at - now);
}

static void	take_one_dongle(t_coder *coder, int dongle_id)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->dongles[dongle_id].mutex);
	wait_dongle_cooldown(sim, &sim->dongles[dongle_id]);
	print_log(sim, coder->id, "has taken a dongle");
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
		take_one_dongle(coder, left);
		while (is_sim_active(sim))
			smart_sleep(sim, 1);
		pthread_mutex_unlock(&sim->dongles[left].mutex);
		return ;
	}
	if (coder->id % 2 == 0)
	{
		take_one_dongle(coder, right);
		take_one_dongle(coder, left);
	}
	else
	{
		take_one_dongle(coder, left);
		take_one_dongle(coder, right);
	}
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;
	long	now;

	sim = coder->sim;
	left = get_left_dongle(coder);
	right = get_right_dongle(coder);
	now = get_time_ms();
	sim->dongles[right].released_at_ms = now;
	pthread_mutex_unlock(&sim->dongles[right].mutex);
	sim->dongles[left].released_at_ms = now;
	pthread_mutex_unlock(&sim->dongles[left].mutex);
}