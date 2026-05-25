#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (is_sim_active(sim))
	{
		print_log(sim, coder->id, "is refactoring");
		usleep(1000 * 500);
		break ;
	}
	return (NULL);
}