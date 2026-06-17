/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:23:29 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_fdf	*init_fdf(char *filename)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)malloc(sizeof(t_fdf));
	if (!fdf)
		error_exit("Error: malloc failed");
	fdf->cam = (t_cam *)malloc(sizeof(t_cam));
	if (!fdf->cam)
		error_exit("Error: malloc failed");
	fdf->map = parse_map(filename);
	fdf->mlx = mlx_init();
	if (!fdf->mlx)
		error_exit("Error: mlx_init failed");
	fdf->win = mlx_new_window(fdf->mlx, WIN_W, WIN_H, "FdF");
	if (!fdf->win)
		error_exit("Error: mlx_new_window failed");
	fdf->img = mlx_new_image(fdf->mlx, WIN_W, WIN_H);
	if (!fdf->img)
		error_exit("Error: mlx_new_image failed");
	fdf->addr = mlx_get_data_addr(fdf->img, &fdf->bpp,
			&fdf->line_len, &fdf->endian);
	init_cam(fdf);
	return (fdf);
}

int	main(int argc, char **argv)
{
	t_fdf	*fdf;

	if (argc != 2)
		error_exit("Usage: ./fdf <map.fdf>");
	fdf = init_fdf(argv[1]);
	draw_map(fdf);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
	mlx_key_hook(fdf->win, key_hook, fdf);
	mlx_hook(fdf->win, MLX_CLOSE_EVENT, 0, (int (*)())close_hook, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
