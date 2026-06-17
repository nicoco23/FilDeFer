/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:23:16 by ntassin           #+#    #+#             */
/*   Updated: 2026/04/29 11:23:27 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	draw_map(t_fdf *fdf)
{
	t_pt	p;
	int		row;
	int		col;

	ft_memset(fdf->addr, 0, WIN_H * fdf->line_len);
	row = 0;
	while (row < fdf->map->height)
	{
		col = 0;
		while (col < fdf->map->width)
		{
			p = map_to_screen(fdf, col, row);
			if (col + 1 < fdf->map->width)
				draw_line(fdf, p, map_to_screen(fdf, col + 1, row));
			if (row + 1 < fdf->map->height)
				draw_line(fdf, p, map_to_screen(fdf, col, row + 1));
			col++;
		}
		row++;
	}
}
