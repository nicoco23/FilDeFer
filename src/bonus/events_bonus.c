/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/15 15:10:02 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

void	quit(t_fdf *fdf)
{
	mlx_destroy_image(fdf->mlx, fdf->img);
	mlx_destroy_window(fdf->mlx, fdf->win);
	mlx_destroy_display(fdf->mlx);
	free(fdf->mlx);
	free_map(fdf->map);
	free(fdf->cam);
	free(fdf);
	exit(0);
}

void	redraw(t_fdf *fdf)
{
	draw_map(fdf);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
	draw_hud(fdf);
}

int	loop_hook(void *param)
{
	t_fdf	*fdf;
	t_tv	now;
	float	delta;

	fdf = (t_fdf *)param;
	gettimeofday(&now, NULL);
	delta = (now.tv_sec - fdf->last_tick.tv_sec) * 1000000.0f
		+ (now.tv_usec - fdf->last_tick.tv_usec);
	if (delta > 0.0f)
		fdf->fps = 1000000.0f / delta;
	fdf->last_tick = now;
	if (fdf->flags & FDF_AUTO_ROTATE)
	{
		fdf->cam->rot_y += 0.02f;
		fdf->flags |= FDF_DIRTY;
	}
	if (fdf->flags & FDF_DIRTY)
	{
		redraw(fdf);
		fdf->flags &= ~FDF_DIRTY;
	}
	return (0);
}

int	close_hook(void *param)
{
	quit((t_fdf *)param);
	return (0);
}
