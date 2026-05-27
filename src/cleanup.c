#include "../include/codexion.h"

void	cleanup_sim(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pqueue_free(&sim->dongles[i].queue);
		i++;
	}
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->active_mutex);
	free(sim->coders);
	free(sim->dongles);
}

void	pqueue_free(t_pqueue *queue)
{
	free(queue->data);
	queue->data = NULL;
	queue->size = 0;
	queue->capacity = 0;
}