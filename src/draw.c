/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:23:06 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	put_pixel(t_fdf *fdf, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	dst = fdf->addr + (y * fdf->line_len + x * (fdf->bpp / 8));
	*(unsigned int *)dst = color;
}

static void	init_line(t_pt p1, t_pt p2, t_line *line, int *steps)
{
	line->dx = ft_abs(p2.x - p1.x);
	line->dy = ft_abs(p2.y - p1.y);
	if (p1.x < p2.x)
		line->sx = 1;
	else
		line->sx = -1;
	if (p1.y < p2.y)
		line->sy = 1;
	else
		line->sy = -1;
	line->err = line->dx - line->dy;
	if (line->dx > line->dy)
		*steps = line->dx;
	else
		*steps = line->dy;
	if (*steps == 0)
		*steps = 1;
}

static void	step_line(t_pt *p, t_line *line, int e2)
{
	if (e2 > -line->dy)
	{
		line->err -= line->dy;
		p->x += line->sx;
	}
	if (e2 < line->dx)
	{
		line->err += line->dx;
		p->y += line->sy;
	}
}

void	draw_line(t_fdf *fdf, t_pt p1, t_pt p2)
{
	t_line	line;
	int		steps;
	int		step;

	init_line(p1, p2, &line, &steps);
	step = 0;
	while (step <= steps)
	{
		put_pixel(fdf, p1.x, p1.y,
			lerp_color(p1.color, p2.color, (float)step / steps));
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		step_line(&p1, &line, 2 * line.err);
		step++;
	}
}

t_pt	map_to_screen(t_fdf *fdf, int col, int row)
{
	t_vec3	gpt;
	t_pt	p;
	float	norm;

	gpt.col = col;
	gpt.row = row;
	gpt.z = fdf->map->z[row][col];
	p = iso_project(gpt, fdf->cam);
	if (fdf->map->color[row][col] != NO_COLOR)
		p.color = fdf->map->color[row][col];
	else if (fdf->map->z_max != fdf->map->z_min)
	{
		norm = (float)(gpt.z - fdf->map->z_min)
			/ (float)(fdf->map->z_max - fdf->map->z_min);
		p.color = lerp_color(0x0000FF, 0xFFFFFF, norm);
	}
	else
		p.color = 0xFFFFFF;
	return (p);
}
