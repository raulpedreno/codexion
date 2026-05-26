#include "../include/codexion.h"

int	pqueue_init(t_pqueue *queue, int capacity)
{
	queue->data = malloc(sizeof(t_waiter) * capacity);
	if (!queue->data)
		return (1);
	queue->size = 0;
	queue->capacity = capacity;
	return (0);
}

void	pqueue_free(t_pqueue *queue)
{
	free(queue->data);
	queue->data = NULL;
	queue->size = 0;
	queue->capacity = 0;
}