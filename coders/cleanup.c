/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:31:59 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:03 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_sim(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	if (sim->dongles && sim->coders)
	{
		while (i < sim->config.number_of_coders)
		{
			pthread_mutex_destroy(&sim->coders[i].state_mutex);
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			pthread_cond_destroy(&sim->dongles[i].cond);
			pqueue_free(&sim->dongles[i].queue);
			i++;
		}
	}
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->active_mutex);
	free(sim->coders);
	free(sim->dongles);
}
