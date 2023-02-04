/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcruz-na <dcruz-na@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 13:05:17 by dcruz-na          #+#    #+#             */
/*   Updated: 2023/02/04 19:40:51 by dcruz-na         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	d;

	if (argc < 5 || argc > 6)
		exit_error("few arguments");
	error_management(argc, argv);
	fill_data(&d, argv, argc);
	destroy_program(&d);
}

void	fill_philo(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->philo_n)
	{
		d->philos[i].n = i;
		d->philos[i].data = d;
		d->philos[i].eat_times = 0;
		d->philos[i].last_eat = -1;
		if (pthread_mutex_init(&(d->philos[i].l_fork), NULL))
			error_free(d, "Mutex failed\n");
		if (i == d->philo_n - 1)
			d->philos[i].r_fork = &(d->philos[0].l_fork);
		else
			d->philos[i].r_fork = &(d->philos[i + 1].l_fork);
		if (pthread_create(&(d->philos[i].thread), NULL, phil, &(d->philos[i])))
			error_free(d, "Thread failed");
	}
	i = -1;
	while (++i < d->philo_n)
		if (pthread_join(d->philos[i].thread, NULL) != 0)
			return ;
}

void	fill_data(t_data *d, char **argv, int argc)
{
	if (pthread_mutex_init(&d->dead, NULL))
		error_free(d, "Mutex failed");
	if (pthread_mutex_init(&d->m_stop, NULL))
		error_free(d, "Mutex failed");
	if (pthread_mutex_init(&d->m_eat, NULL))
		error_free(d, "Mutex failed");
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
	fill_philo(d);
}

void	destroy_program(t_data *d)
{
	int		i;

	i = 0;
	pthread_mutex_destroy(&d->dead);
	pthread_mutex_destroy(&d->m_eat);
	pthread_mutex_destroy(&d->m_stop);
	while (i < d->philo_n)
	{
		pthread_mutex_destroy(&d->philos[i].l_fork);
		i++;
	}
}
