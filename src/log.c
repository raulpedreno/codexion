/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:32:25 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:32:28 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	print_log(t_sim *sim, int coder_id, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	if (is_sim_active(sim) || strcmp(message, "burned out") == 0)
	{
		timestamp = get_time_ms() - sim->start_ms;
		printf("%li %i %s\n", timestamp, coder_id, message);
	}
	pthread_mutex_unlock(&sim->log_mutex);
}
