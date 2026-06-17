/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/04 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

void	update_trig(t_cam *cam)
{
	cam->cx = (float)cos(cam->rot_x);
	cam->sx = (float)sin(cam->rot_x);
	cam->cy = (float)cos(cam->rot_y);
	cam->sy = (float)sin(cam->rot_y);
	cam->cz = (float)cos(cam->rot_z);
	cam->sz = (float)sin(cam->rot_z);
}

t_pt	iso_project(t_vec3 pt, t_cam *cam)
{
	float	x;
	float	y;
	float	z;
	float	tmp;
	t_pt	s;

	x = (float)pt.col;
	y = (float)pt.row;
	z = (float)pt.z;
	tmp = x * cam->cz - y * cam->sz;
	y = x * cam->sz + y * cam->cz;
	x = tmp;
	tmp = y * cam->cx - z * cam->sx;
	z = y * cam->sx + z * cam->cx;
	y = tmp;
	tmp = x * cam->cy + z * cam->sy;
	z = -x * cam->sy + z * cam->cy;
	x = tmp;
	s.x = (int)((x - y) * 0.866f * cam->zoom) + cam->x_off;
	s.y = (int)((x + y) * 0.5f * cam->zoom - z * cam->z_scale) + cam->y_off;
	s.color = 0;
	return (s);
}

t_pt	parallel_project(t_vec3 pt, t_cam *cam)
{
	t_pt	s;

	s.x = (int)(pt.col * cam->zoom) + cam->x_off;
	s.y = (int)(pt.row * cam->zoom - pt.z * cam->z_scale) + cam->y_off;
	s.color = 0;
	return (s);
}

/*
** relief : hauteur visuelle du z_max à l'écran (en unités grille).
** Proportionnel à la taille (6% du span) sinon les grandes maps
** paraissent plates ; plancher à 20 = comportement des petites maps.
*/
void	init_cam(t_fdf *fdf)
{
	float	zw;
	float	zh;
	float	span;
	float	relief;

	fdf->cam->rot_x = 0.0f;
	fdf->cam->rot_y = 0.0f;
	fdf->cam->rot_z = 0.0f;
	fdf->cam->proj = 0;
	span = (float)(fdf->map->width + fdf->map->height);
	zw = (float)WIN_H / (span * 0.866f * 1.1f);
	zh = (float)WIN_H / (span * 0.5f * 1.2f);
	fdf->cam->zoom = fminf(zw, zh);
	if (fdf->cam->zoom < 0.05f)
		fdf->cam->zoom = 0.05f;
	relief = fmaxf(20.0f, span * 0.06f);
	if (fdf->map->z_max - fdf->map->z_min == 0)
		fdf->cam->z_scale = 0;
	else
		fdf->cam->z_scale = fdf->cam->zoom * relief
			/ (float)(fdf->map->z_max - fdf->map->z_min);
	fdf->cam->x_off = WIN_W / 2;
	fdf->cam->y_off = WIN_H / 4;
}
