/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	init_mlx(t_fdf *fdf)
{
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
}

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
	init_mlx(fdf);
	init_cam(fdf);
	fdf->flags = (1 << FDF_SIZE_SHIFT);
	fdf->fps = 0.0f;
	gettimeofday(&fdf->last_tick, NULL);
	fdf->seed = 42;
	return (fdf);
}

int	main(int argc, char **argv)
{
	t_fdf	*fdf;

	if (argc != 2)
		error_exit("Usage: ./fdf_bonus <map.fdf>");
	srand((unsigned int)time(NULL));
	fdf = init_fdf(argv[1]);
	draw_map(fdf);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
	mlx_key_hook(fdf->win, key_hook, fdf);
	mlx_hook(fdf->win, MLX_CLOSE_EVENT, 0, (int (*)())close_hook, fdf);
	mlx_hook(fdf->win, EVT_BTN_PRESS, MASK_BTN_PRESS,
		(int (*)())mouse_hook, fdf);
	mlx_hook(fdf->win, EVT_BTN_RELEASE, MASK_BTN_RELEASE,
		(int (*)())release_hook, fdf);
	mlx_hook(fdf->win, EVT_MOTION, MASK_BTN_MOTION,
		(int (*)())motion_hook, fdf);
	mlx_loop_hook(fdf->mlx, (int (*)())loop_hook, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
