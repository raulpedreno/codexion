/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:32:38 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:39 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_coder_burnout(t_sim *sim, t_coder *coder)
{
	long	now;
	long	last_compile_start;
	long	time_without_compile;
	int		compile_count;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile_start = coder->last_compile_start_ms;
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->state_mutex);
	if (compile_count >= sim->config.number_of_compiles_required)
		return (0);
	now = get_time_ms();
	time_without_compile = now - last_compile_start;
	if (time_without_compile > sim->config.time_to_burnout)
	{
		print_log(sim, coder->id, "burned out");
		stop_sim(sim);
		wake_all_dongles(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (is_sim_active(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (check_coder_burnout(sim, &sim->coders[i]))
				return (NULL);
			i++;
		}
		if (all_coders_done(sim))
		{
			stop_sim(sim);
			wake_all_dongles(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
