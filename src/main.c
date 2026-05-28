/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:32:32 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:34 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	main(int argc, char **argv)
{
	t_sim		sim;
	t_config	config;

	if (parse_args(argc, argv, &config) != 0)
		return (1);
	if (init_sim(&sim, config) != 0)
	{
		cleanup_sim(&sim);
		return (1);
	}
	if (start_threads(&sim) != 0)
	{
		cleanup_sim(&sim);
		return (1);
	}
	if (join_threads(&sim) != 0)
	{
		cleanup_sim(&sim);
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
