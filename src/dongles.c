/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:32:09 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:12 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static t_waiter	create_waiter(t_coder *coder)
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

static int	take_one_dongle(t_coder *coder, int dongle_id)
{
	t_sim		*sim;
	t_dongle	*dongle;
	t_waiter	waiter;
	t_waiter	first;

	sim = coder->sim;
	dongle = &sim->dongles[dongle_id];
	waiter = create_waiter(coder);
	pthread_mutex_lock(&dongle->mutex);
	if (pqueue_push(&dongle->queue, waiter, sim->config.scheduler) != 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	while (is_sim_active(sim))
	{
		if (pqueue_peek(&dongle->queue, &first) != 0)
			break ;
		if (first.coder_id == coder->id && dongle->in_use == 0)
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (!is_sim_active(sim))
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

static void	release_one_dongle(t_sim *sim, int dongle_id)
{
	t_dongle	*dongle;

	dongle = &sim->dongles[dongle_id];
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->released_at_ms = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

int	take_dongles(t_coder *coder)
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
		release_one_dongle(sim, left);
		return (0);
	}
	if (coder->id % 2 == 0)
	{
		if (!take_one_dongle(coder, right))
			return (0);
		if (!take_one_dongle(coder, left))
		{
			release_one_dongle(sim, right);
			return (0);
		}
	}
	else
	{
		if (!take_one_dongle(coder, left))
			return (0);
		if (!take_one_dongle(coder, right))
		{
			release_one_dongle(sim, left);
			return (0);
		}
	}
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;

	sim = coder->sim;
	left = get_left_dongle(coder);
	right = get_right_dongle(coder);
	release_one_dongle(sim, right);
	release_one_dongle(sim, left);
}

void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}
