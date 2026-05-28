/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedreno <rpedreno@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:31:37 by rpedreno          #+#    #+#             */
/*   Updated: 2026/05/28 15:31:40 by rpedreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define SCHEDULER_FIFO 1
# define SCHEDULER_EDF 2

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

//entrada: Cada vez que un coder pide un dongle → creas un t_waiter
typedef struct s_waiter
{
    int     coder_id;
    long    request_time_ms;
    long    deadline_ms;
}   t_waiter;


//cola de un dogle (priority queue)
typedef struct s_pqueue
{
    t_waiter    *data;
    int         size;
    int         capacity;
}   t_pqueue;


// recurso compartido entre coders
typedef struct s_dongle
{
    int                 id;
    int                 in_use;
    long                released_at_ms;
    pthread_mutex_t     mutex;  //El mutex protege todo lo que ocurre entre lock y unlock. pthread_mutex_lock(&dongle->mutex);
    pthread_cond_t      cond;   //Es una herramienta del sistema operativo para dormir y despertar hilos
    t_pqueue            queue;
}   t_dongle;

typedef struct s_coder
{
    int                 id;
    int                 compile_count;
    int                 burned_out;
    long                last_compile_start_ms;
    pthread_t           thread;
    pthread_mutex_t     state_mutex;
    struct s_sim        *sim;
}   t_coder;

typedef struct s_sim
{
    t_config        config;
    t_coder         *coders;
    t_dongle        *dongles;
    int             active;
    long            start_ms;
    pthread_mutex_t log_mutex;
    pthread_mutex_t active_mutex;
    pthread_t       monitor_thread;
}   t_sim;

int	parse_args(int argc, char **argv, t_config *config);
long	get_time_ms(void);
int	    init_sim(t_sim *sim, t_config config);
void	cleanup_sim(t_sim *sim);
void	print_log(t_sim *sim, int coder_id, char *message);
int		is_sim_active(t_sim *sim);
void	stop_sim(t_sim *sim);
void	*coder_routine(void *arg);
int	    start_threads(t_sim *sim);
int	    join_threads(t_sim *sim);
void	*monitor_routine(void *arg);
int 	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
int	    all_coders_done(t_sim *sim);
void	smart_sleep(t_sim *sim, long duration_ms);

int		pqueue_init(t_pqueue *queue, int capacity);
void	pqueue_free(t_pqueue *queue);
int		pqueue_push(t_pqueue *queue, t_waiter waiter, int scheduler);
int		pqueue_pop(t_pqueue *queue, t_waiter *out);
int		pqueue_peek(t_pqueue *queue, t_waiter *out);
int     pqueue_remove_by_coder(t_pqueue *queue, int coder_id);
void	wake_all_dongles(t_sim *sim);


#endif