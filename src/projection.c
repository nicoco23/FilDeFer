/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:22:55 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** Projection isométrique fixe :
**   x_screen = (col - row) * cos(30°) * zoom
**   y_screen = (col + row) * sin(30°) * zoom - z * z_scale
** cos(30°) ≈ 0.866, sin(30°) = 0.5.
*/
t_pt	iso_project(t_vec3 pt, t_cam *cam)
{
	t_pt	s;

	s.x = (int)((pt.col - pt.row) * 0.866f * cam->zoom) + cam->x_off;
	s.y = (int)((pt.col + pt.row) * 0.5f * cam->zoom
			- pt.z * cam->z_scale) + cam->y_off;
	s.color = 0;
	return (s);
}

void	init_cam(t_fdf *fdf)
{
	float	zw;
	float	zh;
	float	span;

	span = (float)(fdf->map->width + fdf->map->height);
	zw = (float)WIN_W / (span * 0.866f * 1.2f);
	zh = (float)WIN_H / (span * 0.5f * 1.4f);
	fdf->cam->zoom = fminf(zw, zh);
	if (fdf->cam->zoom < 1.0f)
		fdf->cam->zoom = 1.0f;
	if (fdf->map->z_max - fdf->map->z_min == 0)
		fdf->cam->z_scale = 0.0f;
	else
		fdf->cam->z_scale = fdf->cam->zoom
			/ (float)(fdf->map->z_max - fdf->map->z_min) * 20.0f;
	fdf->cam->x_off = WIN_W / 2;
	fdf->cam->y_off = WIN_H / 4;
}
