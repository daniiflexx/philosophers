/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcruz-na <dcruz-na@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:21:42 by danicn            #+#    #+#             */
/*   Updated: 2023/02/04 19:06:24 by dcruz-na         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_free(t_data *d, char *msj)
{
	destroy_program(d);
	exit_error(msj);
}

void	exit_error(char *msj)
{
	perror(msj);
	exit(EXIT_FAILURE);
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
