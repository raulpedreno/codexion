/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:38:26 by rpedreno          #+#    #+#             */
/*   Updated: 2026/06/02 16:40:15 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	wait_turn(t_coder *coder, t_dongle *dongle)
{
	t_waiter	first;

	while (is_sim_active(coder->sim))
	{
		if (pqueue_peek(&dongle->queue, &first) != 0)
			break ;
		if (first.coder_id == coder->id && dongle->in_use == 0)
			return (1);
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	return (0);
}

int	take_one_dongle(t_coder *coder, int dongle_id)
{
	t_sim		*sim;
	t_dongle	*dongle;
	t_waiter	waiter;

	sim = coder->sim;
	dongle = &sim->dongles[dongle_id];
	waiter = create_waiter(coder);
	pthread_mutex_lock(&dongle->mutex);
	if (pqueue_push(&dongle->queue, waiter, sim->config.scheduler) != 0)
		return (pthread_mutex_unlock(&dongle->mutex), 0);
	if (!wait_turn(coder, dongle))
	{
		pqueue_remove_by_coder(&dongle->queue, coder->id);
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	pqueue_remove_by_coder(&dongle->queue, coder->id);
	dongle->in_use = 1;
	pthread_cond_broadcast(&dongle->cond);
	wait_dongle_cooldown(sim, dongle);
	print_log(sim, coder->id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}
