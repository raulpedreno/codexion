#include "../include/codexion.h"

static int	is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	ft_atol(char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

static int	parse_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (SCHED_FIFO);
	if (strcmp(str, "edf") == 0)
		return (SCHED_EDF);
	return (0);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (1);
	if (!is_number(argv[1]) || !is_number(argv[2])
		|| !is_number(argv[3]) || !is_number(argv[4])
		|| !is_number(argv[5]) || !is_number(argv[6])
		|| !is_number(argv[7]))
		return (1);
	config->number_of_coders = ft_atol(argv[1]);
	config->time_to_burnout = ft_atol(argv[2]);
	config->time_to_compile = ft_atol(argv[3]);
	config->time_to_debug = ft_atol(argv[4]);
	config->time_to_refactor = ft_atol(argv[5]);
	config->number_of_compiles_required = ft_atol(argv[6]);
	config->dongle_cooldown = ft_atol(argv[7]);
	config->scheduler = parse_scheduler(argv[8]);
	if (config->scheduler == 0)
		return (1);
	if (config->number_of_coders <= 0
		|| config->time_to_burnout <= 0
		|| config->time_to_compile <= 0
		|| config->time_to_debug <= 0
		|| config->time_to_refactor <= 0
		|| config->number_of_compiles_required <= 0
		|| config->dongle_cooldown < 0)
		return (1);
	return (0);
}