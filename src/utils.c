/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:33:21 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 16:03:55 by rpedreno         ###   ########.fr       */
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

int	pqueue_remove_by_coder(t_pqueue *queue, int coder_id)
{
	int	i;
	int	j;

	i = 0;
	while (i < queue->size)
	{
		if (queue->data[i].coder_id == coder_id)
		{
			j = i + 1;
			while (j < queue->size)
			{
				queue->data[j - 1] = queue->data[j];
				j++;
			}
			queue->size--;
			return (0);
		}
		i++;
	}
	return (1);
}

void	pqueue_free(t_pqueue *queue)
{
	free(queue->data);
	queue->data = NULL;
	queue->size = 0;
	queue->capacity = 0;
}
