#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (is_sim_active(sim))
	{
		coder->last_compile_start_ms = get_time_ms();
		print_log(sim, coder->id, "is compiling");
		usleep(sim->config.time_to_compile * 1000);
		coder->compile_count++;

		print_log(sim, coder->id, "is debugging");
		usleep(sim->config.time_to_debug * 1000);

		print_log(sim, coder->id, "is refactoring");
		usleep(sim->config.time_to_refactor * 1000);

		if (coder->compile_count >= sim->config.number_of_compiles_required)
			break ;
	}
	return (NULL);
}