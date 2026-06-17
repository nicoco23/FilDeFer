/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:23:16 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	quit(t_fdf *fdf)
{
	if (fdf->img)
		mlx_destroy_image(fdf->mlx, fdf->img);
	if (fdf->win)
		mlx_destroy_window(fdf->mlx, fdf->win);
	mlx_destroy_display(fdf->mlx);
	free(fdf->mlx);
	free_map(fdf->map);
	free(fdf->cam);
	free(fdf);
	exit(0);
}

int	key_hook(int key, void *param)
{
	if (key == KEY_ESC)
		quit((t_fdf *)param);
	return (0);
}

int	close_hook(void *param)
{
	quit((t_fdf *)param);
	return (0);
}
