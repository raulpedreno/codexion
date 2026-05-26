#include "../include/codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv; //estructura del SO

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));  //devuelve todo los milisegundos
}

// sustituye usleep comprueba que ese hilo no esta agotado
void	smart_sleep(t_sim *sim, long duration_ms)
{
	long	start;

	start = get_time_ms();
	while (is_sim_active(sim)
		&& get_time_ms() - start < duration_ms)
		usleep(500);
}