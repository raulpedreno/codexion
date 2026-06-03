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

#include "codexion.h"

static int	take_even_dongles(t_coder *coder, int left, int right)
{
	t_sim	*sim;

	sim = coder->sim;
	if (!take_one_dongle(coder, right))
		return (0);
	if (!take_one_dongle(coder, left))
	{
		release_one_dongle(sim, right);
		return (0);
	}
	return (1);
}

static int	take_odd_dongles(t_coder *coder, int left, int right)
{
	t_sim	*sim;

	sim = coder->sim;
	if (!take_one_dongle(coder, left))
		return (0);
	if (!take_one_dongle(coder, right))
	{
		release_one_dongle(sim, left);
		return (0);
	}
	return (1);
}

static int	take_single_dongle(t_coder *coder, int left)
{
	t_sim	*sim;

	sim = coder->sim;
	take_one_dongle(coder, left);
	while (is_sim_active(sim))
		smart_sleep(sim, 1);
	release_one_dongle(sim, left);
	return (0);
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
		return (take_single_dongle(coder, left));
	if (coder->id % 2 == 0)
		return (take_even_dongles(coder, left, right));
	return (take_odd_dongles(coder, left, right));
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
