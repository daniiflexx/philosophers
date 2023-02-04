/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcruz-na <dcruz-na@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:16:52 by danicn            #+#    #+#             */
/*   Updated: 2023/02/04 19:47:37 by dcruz-na         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msleep(unsigned int ms, t_data *data)
{
	if (data->philo_n > 20)
		(usleep(ms * 1000));
	else
		ft_usleep(ms);
}

unsigned long	my_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void	ft_usleep(unsigned long time)
{
	long long	start;

	start = my_time();
	while (my_time() < start + time)
		usleep(10);
}

unsigned long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000LL + t.tv_usec / 1000));
}

int	it_is_dead(t_philo *philo, int nb)
{
	pthread_mutex_lock(&philo->data->dead);
	if (nb)
	{
		pthread_mutex_lock(&philo->data->m_stop);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->m_stop);
	}
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead);
	return (0);
}
