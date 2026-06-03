/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:33:09 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 16:03:55 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	waiter_goes_before(t_waiter a, t_waiter b, int scheduler)
{
	if (scheduler == SCHEDULER_FIFO)
		return (a.request_time_ms < b.request_time_ms);
	if (scheduler == SCHEDULER_EDF)
		return (a.deadline_ms < b.deadline_ms);
	return (0);
}

static void	pqueue_sort(t_pqueue *queue, int scheduler)
{
	int			i;
	int			j;
	t_waiter	tmp;

	i = 0;
	while (i < queue->size)
	{
		j = i + 1;
		while (j < queue->size)
		{
			if (waiter_goes_before(queue->data[j], queue->data[i], scheduler))
			{
				tmp = queue->data[i];
				queue->data[i] = queue->data[j];
				queue->data[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	pqueue_push(t_pqueue *queue, t_waiter waiter, int scheduler)
{
	if (queue->size >= queue->capacity)
		return (1);
	queue->data[queue->size] = waiter;
	queue->size++;
	pqueue_sort(queue, scheduler);
	return (0);
}

int	pqueue_peek(t_pqueue *queue, t_waiter *out)
{
	if (queue->size <= 0)
		return (1);
	*out = queue->data[0];
	return (0);
}

int	pqueue_pop(t_pqueue *queue, t_waiter *out)
{
	int	i;

	if (queue->size <= 0)
		return (1);
	*out = queue->data[0];
	i = 1;
	while (i < queue->size)
	{
		queue->data[i - 1] = queue->data[i];
		i++;
	}
	queue->size--;
	return (0);
}
