/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 12:32:37 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/16 16:33:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	apply_rotation(int dx, int dy, t_fdf *fdf)
{
	t_vec3	center;
	t_pt	before;
	t_pt	after;

	center.col = fdf->map->width / 2;
	center.row = fdf->map->height / 2;
	center.z = 0;
	before = iso_project(center, fdf->cam);
	fdf->cam->rot_y += dx * 0.005f;
	fdf->cam->rot_x += dy * 0.005f;
	after = iso_project(center, fdf->cam);
	fdf->cam->x_off += before.x - after.x;
	fdf->cam->y_off += before.y - after.y;
}

static int	handle_click_start(int btn, int x, int y, t_fdf *fdf)
{
	if (btn == MOUSE_LEFT)
		fdf->flags |= FDF_DRAGGING;
	else if (btn == MOUSE_RIGHT)
		fdf->flags |= FDF_ROTATING;
	else
		return (0);
	fdf->drag_x = x;
	fdf->drag_y = y;
	return (1);
}

int	mouse_hook(int btn, int x, int y, void *param)
{
	t_fdf	*fdf;
	float	factor;

	fdf = (t_fdf *)param;
	if (handle_click_start(btn, x, y, fdf))
		return (0);
	if (btn == MOUSE_UP)
		factor = 1.1f;
	else if (btn == MOUSE_DOWN)
		factor = 1.0f / 1.1f;
	else
		return (0);
	fdf->cam->x_off += (int)((x - fdf->cam->x_off) * (1.0f - factor));
	fdf->cam->y_off += (int)((y - fdf->cam->y_off) * (1.0f - factor));
	fdf->cam->zoom *= factor;
	fdf->flags |= FDF_DIRTY;
	return (0);
}

int	release_hook(int btn, int x, int y, void *param)
{
	(void)x;
	(void)y;
	if (btn == MOUSE_LEFT)
		((t_fdf *)param)->flags &= ~FDF_DRAGGING;
	else if (btn == MOUSE_RIGHT)
		((t_fdf *)param)->flags &= ~FDF_ROTATING;
	return (0);
}

int	motion_hook(int x, int y, void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	if (fdf->flags & FDF_DRAGGING)
	{
		fdf->cam->x_off += x - fdf->drag_x;
		fdf->cam->y_off += y - fdf->drag_y;
	}
	else if (fdf->flags & FDF_ROTATING)
		apply_rotation(x - fdf->drag_x, y - fdf->drag_y, fdf);
	else
		return (0);
	fdf->drag_x = x;
	fdf->drag_y = y;
	fdf->flags |= FDF_DIRTY;
	return (0);
}
