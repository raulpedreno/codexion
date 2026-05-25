#include "../include/codexion.h"

void	print_log(t_sim *sim, int coder_id, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_time_ms() - sim->start_ms;
	printf("%li %i %s\n", timestamp, coder_id, message);
	pthread_mutex_unlock(&sim->log_mutex);
}