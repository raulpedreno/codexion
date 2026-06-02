/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:32:09 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:12 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"


int	get_left_dongle(t_coder *coder)
{
	return (coder->id - 1);
}

int	get_right_dongle(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	return (coder->id % sim->config.number_of_coders);
}

void	wait_dongle_cooldown(t_sim *sim, t_dongle *dongle)
{
	long	now;
	long	available_at;

	now = get_time_ms();
	available_at = dongle->released_at_ms + sim->config.dongle_cooldown;
	if (now < available_at)
		smart_sleep(sim, available_at - now);
}

t_waiter	create_waiter(t_coder *coder)
{
	t_waiter	waiter;
	t_sim		*sim;
	long		now;
	long		last_compile_start;

	sim = coder->sim;
	now = get_time_ms();
	pthread_mutex_lock(&coder->state_mutex);
	last_compile_start = coder->last_compile_start_ms;
	pthread_mutex_unlock(&coder->state_mutex);
	waiter.coder_id = coder->id;
	waiter.request_time_ms = now;
	waiter.deadline_ms = last_compile_start + sim->config.time_to_burnout;
	return (waiter);
}

void	release_one_dongle(t_sim *sim, int dongle_id)
{
	t_dongle	*dongle;

	dongle = &sim->dongles[dongle_id];
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->released_at_ms = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}