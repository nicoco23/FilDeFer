/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:23:16 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static int	pick_color(t_fdf *fdf, int col, int row, float norm)
{
	if (fdf_theme(fdf) == 1)
		return (0xFFFFFF);
	if (fdf_theme(fdf) == 2)
		return (rainbow_color(norm));
	if (fdf->map->color[row][col] != NO_COLOR)
		return (fdf->map->color[row][col]);
	if (fdf->map->z_max != fdf->map->z_min)
		return (lerp_color(0x0000FF, 0xFFFFFF, norm));
	return (0xFFFFFF);
}

t_pt	map_to_screen(t_fdf *fdf, int col, int row)
{
	t_vec3	gpt;
	t_pt	p;
	float	norm;

	norm = 0.0f;
	gpt.col = col;
	gpt.row = row;
	gpt.z = fdf->map->z[row][col];
	if (fdf->cam->proj == 0)
		p = iso_project(gpt, fdf->cam);
	else
		p = parallel_project(gpt, fdf->cam);
	if (fdf->map->z_max != fdf->map->z_min)
		norm = (float)(fdf->map->z[row][col] - fdf->map->z_min)
			/ (float)(fdf->map->z_max - fdf->map->z_min);
	p.color = pick_color(fdf, col, row, norm);
	return (p);
}

static void	hud_line(t_fdf *fdf, int y, char *label, int val)
{
	char	*num;

	mlx_string_put(fdf->mlx, fdf->win, 10, y, 0xAAAAAA, label);
	num = ft_itoa(val);
	if (num)
	{
		mlx_string_put(fdf->mlx, fdf->win, 145, y, 0xFFFF00, num);
		free(num);
	}
}

static void	draw_controls(t_fdf *fdf)
{
	mlx_string_put(fdf->mlx, fdf->win, 10, 106, 0x555555,
		"WASD/QE:rot  clic droit:rot");
	mlx_string_put(fdf->mlx, fdf->win, 10, 121, 0x555555,
		"+/-:zoom  []:z_scale");
	mlx_string_put(fdf->mlx, fdf->win, 10, 136, 0x555555,
		"P:proj  C:col  R:reset");
	mlx_string_put(fdf->mlx, fdf->win, 10, 151, 0x555555,
		"SPACE:spin  F12:screenshot");
}

void	draw_hud(t_fdf *fdf)
{
	static char	*pn[5] = {"Mountain", "Island",
		"Desert", "Forest", "Fjords"};
	static char	*sz[5] = {"100 x 100", "200 x 200",
		"300 x 300", "500 x 500", "1000 x 1000"};

	mlx_string_put(fdf->mlx, fdf->win, 10, 12, 0x00CCFF,
		"== MAP GENERATOR ==");
	mlx_string_put(fdf->mlx, fdf->win, 10, 34, 0xAAAAAA, "Preset  [1-5] :");
	mlx_string_put(fdf->mlx, fdf->win, 145, 34, 0xFF8800,
		pn[fdf_preset(fdf)]);
	mlx_string_put(fdf->mlx, fdf->win, 10, 54, 0xAAAAAA, "Taille  [TAB] :");
	mlx_string_put(fdf->mlx, fdf->win, 145, 54, 0x00FF88,
		sz[fdf_size(fdf)]);
	hud_line(fdf, 74, "Seed    [G/N] :", fdf->seed);
	hud_line(fdf, 90, "FPS           :", (int)fdf->fps);
	draw_controls(fdf);
}
