/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcruz-na <dcruz-na@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:17:03 by danicn            #+#    #+#             */
/*   Updated: 2023/02/04 19:40:58 by dcruz-na         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_death(void *phi)
{
	t_philo	*philo;

	philo = (t_philo *)phi;
	msleep(philo->data->time_die + 1, philo->data);
	pthread_mutex_lock(&philo->data->m_eat);
	pthread_mutex_lock(&philo->data->m_stop);
	if (!it_is_dead(philo, 0) && timestamp() - philo->last_eat
		- philo->data->first >= (unsigned long)(philo->data->time_die))
	{
		if (philo->data->stop++ < 2)
			printf("%lli %d died\n", timestamp()
				- philo->data->first, philo->n + 1);
		pthread_mutex_unlock(&philo->data->m_eat);
		pthread_mutex_unlock(&philo->data->m_stop);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->m_eat);
	pthread_mutex_unlock(&philo->data->m_stop);
	return (NULL);
}

void	*phil(void *p)
{
	t_philo		*ph;
	pthread_t	t;

	ph = (t_philo *)p;
	if (ph->n % 2 == 0)
		msleep(ph->data->time_eat / 10, ph->data);
	while (!it_is_dead(ph, 0))
	{
		if (pthread_create(&t, NULL, check_death, ph))
			error_free(ph->data, "Mutex failed");
		eat_and_sleep(ph);
		pthread_detach(t);
		if (ph->eat_times == ph->data->eat_times)
		{
			pthread_mutex_lock(&ph->data->m_stop);
			if (ph->data->philo_eat == ph->data->philo_n)
			{
				++ph->data->stop;
				pthread_mutex_unlock(&ph->data->m_stop);
				return (0);
			}
			pthread_mutex_unlock(&ph->data->m_stop);
		}
	}
	return (NULL);
}

void	take_forks(t_philo *ph)
{
	pthread_mutex_lock(&(ph->l_fork));
	if (ph->data->first != 0)
	{
		if (ph->data->stop < 1)
			printf("%lli %d has taken a fork\n", timestamp()
				- ph->data->first, ph->n + 1);
	}
	else
	{
		ph->data->first = timestamp();
		if (ph->data->stop < 1)
			printf("%lli %d has taken a fork\n", ph->data->first
				- ph->data->first, ph->n + 1);
	}
	if (ph->data->philo_n == 1)
	{
		msleep(ph->data->time_die * 2, ph->data);
		return ;
	}
	pthread_mutex_lock(ph->r_fork);
	if (ph->data->stop < 1)
		printf("%lli %d has taken a fork\n", timestamp()
			- ph->data->first, ph->n + 1);
}

void	sleeping(t_philo *ph)
{
	if (ph->data->stop < 1)
		printf("%lli %d is sleeping\n", timestamp()
			- ph->data->first, ph->n + 1);
	msleep(ph->data->time_sleep, ph->data);
	if (ph->data->stop < 1)
		printf("%lli %d is thinking\n", timestamp()
			- ph->data->first, ph->n + 1);
}

void	eat_and_sleep(t_philo *ph)
{
	take_forks(ph);
	pthread_mutex_lock(&(ph->data->m_eat));
	ph->last_eat = timestamp() - ph->data->first;
	if (ph->data->stop < 1)
		printf("%lli %d is eating\n", ph->last_eat, ph->n + 1);
	ph->eat_times++;
	pthread_mutex_lock(&ph->data->m_stop);
	if (ph->eat_times == ph->data->eat_times)
		ph->data->philo_eat++;
	if (ph->data->philo_eat == ph->data->philo_n)
	{
		ph->data->stop++;
		pthread_mutex_unlock(&ph->data->m_stop);
		pthread_mutex_unlock(&(ph->data->m_eat));
		pthread_mutex_unlock(&(ph->l_fork));
		pthread_mutex_unlock(ph->r_fork);
		return ;
	}
	pthread_mutex_unlock(&ph->data->m_stop);
	pthread_mutex_unlock(&(ph->data->m_eat));
	msleep(ph->data->time_eat, ph->data);
	pthread_mutex_unlock(&(ph->l_fork));
	pthread_mutex_unlock(ph->r_fork);
	sleeping(ph);
}
