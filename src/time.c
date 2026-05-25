#include "../include/codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv; //estructura del SO

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));  //devuelve todo los milisegundos
}