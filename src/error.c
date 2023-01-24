/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicn <danicn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:21:42 by danicn            #+#    #+#             */
/*   Updated: 2023/01/23 12:17:45 by danicn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	exit_error(char *msj)
{
    perror(msj);
    exit(EXIT_FAILURE);
}

void	destroy_programm(t_data *d)
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

void	error_management(int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (++i < argc)
	{
		j = -1;
		while (argv[i][++j] != 0)
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				exit_error("Non positive numeric parameters\n");
		}
	}
}