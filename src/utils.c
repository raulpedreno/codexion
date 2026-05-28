/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:33:21 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:33:22 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	all_coders_done(t_sim *sim)
{
	int	i;
	int	compile_count;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		compile_count = sim->coders[i].compile_count;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		if (compile_count < sim->config.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}