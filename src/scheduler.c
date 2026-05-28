/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:33:09 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:33:10 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

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

//	push → meter una petición
int	pqueue_push(t_pqueue *queue, t_waiter waiter, int scheduler)
{
	if (queue->size >= queue->capacity)
		return (1);
	queue->data[queue->size] = waiter;
	queue->size++;
	pqueue_sort(queue, scheduler);
	return (0);
}

//	peek → mirar quién va primero
int	pqueue_peek(t_pqueue *queue, t_waiter *out)
{
	if (queue->size <= 0)
		return (1);
	*out = queue->data[0];
	return (0);
}

//	pop  → sacar al primero
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