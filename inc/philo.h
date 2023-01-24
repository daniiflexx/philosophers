#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "libft.h"

#define MAX_PHILOS 200

typedef struct s_philo t_philo;
typedef struct s_data t_data;

struct s_philo {
    int n;
    long long last_eat;
    int eat_times;
    pthread_t thread;
    pthread_mutex_t l_fork;
    pthread_mutex_t *r_fork;
    t_data *data;
} ;

struct s_data {
    long long first;
    int philo_n;
    int time_die;
    int time_eat;
    int time_sleep;
    int eat_times;
    int i;
    int stop;
	int philo_eat;
    pthread_mutex_t eating;
    pthread_mutex_t m_eat;
    pthread_mutex_t m_stop;
    pthread_mutex_t dead;

    t_philo philos[MAX_PHILOS];
};

void		exit_error(char *msj);
int			msleep(unsigned int ms);
void	    fill_data(t_data *d, char **argv, int argc);
void        *routine1(void *p);
void	    destroy_programm(t_data *d);
long long	timestamp(void);
void	    error_management(int argc, char **argv);
int			is_turn(t_philo *ph);
void		eat_and_sleep(t_philo *ph);
void		is_dead(t_philo *ph);
void		*check_death(void *phi);
int			it_is_dead(t_philo *philo, int nb);

#endif