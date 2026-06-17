/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 16:32:06 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/16 16:32:17 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	fill_row(t_fdf *fdf, t_pt *buf, int row)
{
	int	col;

	col = 0;
	while (col < fdf->map->width)
	{
		buf[col] = map_to_screen(fdf, col, row);
		col++;
	}
}

static void	draw_row_segs(t_fdf *fdf, t_pt *cur, t_pt *nxt, int row)
{
	int	col;

	col = 0;
	while (col < fdf->map->width)
	{
		if (col + 1 < fdf->map->width
			&& (cur[col].x != cur[col + 1].x
				|| cur[col].y != cur[col + 1].y))
			draw_line(fdf, cur[col], cur[col + 1]);
		if (row + 1 < fdf->map->height
			&& (cur[col].x != nxt[col].x || cur[col].y != nxt[col].y))
			draw_line(fdf, cur[col], nxt[col]);
		col++;
	}
}

static void	draw_loop(t_fdf *fdf, t_pt *cur, t_pt *nxt)
{
	t_pt	*tmp;
	int		row;

	fill_row(fdf, cur, 0);
	row = 0;
	while (row < fdf->map->height)
	{
		if (row + 1 < fdf->map->height)
			fill_row(fdf, nxt, row + 1);
		draw_row_segs(fdf, cur, nxt, row);
		tmp = cur;
		cur = nxt;
		nxt = tmp;
		row++;
	}
}

void	draw_map(t_fdf *fdf)
{
	t_pt	*cur;
	t_pt	*nxt;

	ft_memset(fdf->addr, 0, WIN_H * fdf->line_len);
	update_trig(fdf->cam);
	cur = malloc(fdf->map->width * sizeof(t_pt));
	nxt = malloc(fdf->map->width * sizeof(t_pt));
	if (!cur || !nxt)
	{
		free(cur);
		free(nxt);
		return ;
	}
	draw_loop(fdf, cur, nxt);
	free(cur);
	free(nxt);
}
