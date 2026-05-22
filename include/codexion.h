#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>

# define SCHED_FIFO 1
# define SCHED_EDF 2

typedef struct s_config
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	int		scheduler;
}	t_config;

int	parse_args(int argc, char **argv, t_config *config);

#endif