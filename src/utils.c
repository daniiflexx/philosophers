/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danicn <danicn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 16:16:52 by danicn            #+#    #+#             */
/*   Updated: 2022/12/30 16:26:02 by danicn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int msleep(unsigned int ms)
{
    return (usleep(ms*1000));
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec*1000LL + t.tv_usec/1000));
}
