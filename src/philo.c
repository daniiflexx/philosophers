/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicn <danicn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:17:03 by danicn            #+#    #+#             */
/*   Updated: 2023/01/23 12:31:59 by danicn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_death(void *phi)
{
	t_philo	*philo;

	philo = (t_philo *)phi;
	msleep(philo->data->time_die + 1);
	pthread_mutex_lock(&philo->data->m_eat);
	pthread_mutex_lock(&philo->data->m_stop);
	if (!it_is_dead(philo, 0) && timestamp() - \
			philo->last_eat - philo->data->first >= (long)(philo->data->time_die))
	{
			pthread_mutex_unlock(&philo->data->m_eat);
		pthread_mutex_unlock(&philo->data->m_stop);
		printf("%lli %d died\n", timestamp() - philo->data->first, philo->n + 1);
		destroy_programm(philo->data);
		exit(0);
	}
	pthread_mutex_unlock(&philo->data->m_eat);
	pthread_mutex_unlock(&philo->data->m_stop);
	return (NULL);
}

int	it_is_dead(t_philo *philo, int nb)
{
	pthread_mutex_lock(&philo->data->dead);
	if (nb == 2)
		philo->data->stop = 2;
	else if (nb)
		philo->data->stop = 1;
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead);
	return (0);
}

void	is_dead(t_philo *ph)
{	
	long long	time;

	time = timestamp() - ph->data->first - ph->last_eat;
    if (ph->last_eat > -1 && ph->data->first != 0 && time >= ph->data->time_die)
    {
        printf("%lld %d is dead\n", time, ph->n + 1);
        destroy_programm(ph->data);
        exit(0);
    }
}

void	fill_data(t_data *d, char **argv, int argc)
{
    int     i;

	pthread_mutex_init(&d->dead, NULL);
	pthread_mutex_init(&d->m_stop, NULL);
	pthread_mutex_init(&d->m_eat, NULL);
	d->philo_eat = 0;
    d->i = 0;
	d->stop = 0;
	d->philo_n = ft_atoi(argv[1]);
	d->time_die = ft_atoi(argv[2]);
	d->time_eat = ft_atoi(argv[3]);
	d->time_sleep = ft_atoi(argv[4]);
    d->first = 0;
	if (argc == 6)
		d->eat_times = ft_atoi(argv[5]);
	else
		d->eat_times = -1;
    if (d->philo_n > 200)
        exit_error("Too much philosophers");
    i = -1;
    while (++i < d->philo_n)
    {

        d->philos[i].n = i;
        d->philos[i].data = d;
        d->philos[i].eat_times = 0;
        d->philos[i].last_eat = -1;
        pthread_mutex_init(&(d->philos[i].l_fork), NULL);
        if (i == d->philo_n - 1)
			d->philos[i].r_fork = &(d->philos[0].l_fork);
		else
			d->philos[i].r_fork = &(d->philos[i + 1].l_fork);
        pthread_create(&(d->philos[i].thread), NULL, routine1, &(d->philos[i]));

    }
    i = -1;
	while (++i < d->philo_n)
		if (pthread_join(d->philos[i].thread, NULL) != 0)
			return ;
}

void	eat_and_sleep(t_philo *ph)
{   
	pthread_mutex_lock(&(ph->l_fork));
	if (ph->data->first != 0)
        printf("%lli %d has taken a fork\n", timestamp() - ph->data->first, ph->n+1);
    else
    {
        ph->data->first = timestamp();
        printf("%lli %d has taken a fork\n", ph->data->first - ph->data->first, ph->n+1);
	}
	if (ph->data->philo_n == 1)
	{
		msleep(ph->data->time_die * 2);
		return ;
	}
    pthread_mutex_lock(ph->r_fork);
    printf("%lli %d has taken a fork\n", timestamp() - ph->data->first, ph->n+1);
	pthread_mutex_lock(&(ph->data->m_eat));
	ph->last_eat = timestamp() - ph->data->first;
    printf("%lli %d is eating\n", ph->last_eat, ph->n+1);
    ph->eat_times++;
	pthread_mutex_lock(&ph->data->m_stop);
	if (ph->eat_times == ph->data->eat_times)
		ph->data->philo_eat++;
	pthread_mutex_unlock(&ph->data->m_stop);
	pthread_mutex_unlock(&(ph->data->m_eat));
    msleep(ph->data->time_eat);		
    pthread_mutex_unlock(&(ph->l_fork));
    pthread_mutex_unlock(ph->r_fork);
	if (ph->data->philo_eat == ph->data->philo_n)
		return ;
	
    printf("%lli %d is sleeping\n", timestamp() - ph->data->first, ph->n+1);
    msleep(ph->data->time_sleep);
    printf("%lli %d is thinking\n", timestamp() - ph->data->first, ph->n+1);
}

int is_turn(t_philo *ph)
{
    int i;

    i = 0;
    while (i < ph->data->philo_n)
        if (ph->data->philos[i++].eat_times < ph->eat_times)
            return (0);
    return (1);
}

void	*routine1(void *p) {
    t_philo	*ph;
	pthread_t t;
	
    ph = (t_philo *)p;
	if (ph->n % 2 == 0)
    	msleep(ph->data->time_eat / 10);
	while (!it_is_dead(ph, 0))
	{
		pthread_create(&t, NULL, check_death, ph);
		eat_and_sleep(ph);
		pthread_detach(t);
		if (ph->eat_times == ph->data->eat_times)
		{
			pthread_mutex_lock(&ph->data->m_stop);
			if (ph->data->philo_eat == ph->data->philo_n)
			{
				pthread_mutex_unlock(&ph->data->m_stop);
				destroy_programm(ph->data);
				exit(0);
			}
			pthread_mutex_unlock(&ph->data->m_stop);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int argc, char **argv) {
	t_data d;

    if (argc < 5 || argc > 6)
        exit_error("few arguments");
    error_management(argc, argv);
    fill_data(&d, argv, argc);
}


// int i = 0;
//     pthread_t t1, t2;
//     pthread_mutex_t m;

//     pthread_mutex_init(&m, NULL);
//     pthread_create(&t1, NULL, routine1, &m);
//     pthread_create(&t2, NULL, routine2, &m);
//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);
//     printf("value %d\n", x);
//     pthread_mutex_destroy(&m);