/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:22:23 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	error_exit(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	exit(1);
}

static void	free_2d_array(int **arr, int height)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < height)
		free(arr[i++]);
	free(arr);
}

void	free_map(t_map *map)
{
	if (!map)
		return ;
	free_2d_array(map->z, map->height);
	free_2d_array(map->color, map->height);
	free(map);
}

int	lerp_color(int c1, int c2, float t)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((c1 >> 16) & 0xFF) * (1.0f - t) + ((c2 >> 16) & 0xFF) * t);
	g = (int)(((c1 >> 8) & 0xFF) * (1.0f - t) + ((c2 >> 8) & 0xFF) * t);
	b = (int)((c1 & 0xFF) * (1.0f - t) + (c2 & 0xFF) * t);
	return ((r << 16) | (g << 8) | b);
}

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}
