#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (is_sim_active(sim))
	{
		take_dongles(coder);

		pthread_mutex_lock(&coder->state_mutex);
		coder->last_compile_start_ms = get_time_ms();
		pthread_mutex_unlock(&coder->state_mutex);
		
		print_log(sim, coder->id, "is compiling");
		smart_sleep(sim, sim->config.time_to_compile);

		pthread_mutex_lock(&coder->state_mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->state_mutex);


		release_dongles(coder);

		print_log(sim, coder->id, "is debugging");
		smart_sleep(sim, sim->config.time_to_debug);

		print_log(sim, coder->id, "is refactoring");
		smart_sleep(sim, sim->config.time_to_refactor);

		if (all_coders_done(sim))
		{
			stop_sim(sim);
			break ;
		}
	}
	return (NULL);
}