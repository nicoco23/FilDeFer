/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"
#include "mapgen_bonus.h"

static void	handle_rotation(int key, t_fdf *fdf)
{
	if (key == KEY_Q)
		fdf->cam->rot_z -= 0.05f;
	else if (key == KEY_E)
		fdf->cam->rot_z += 0.05f;
	else if (key == KEY_W)
		fdf->cam->rot_x -= 0.05f;
	else if (key == KEY_S)
		fdf->cam->rot_x += 0.05f;
	else if (key == KEY_A)
		fdf->cam->rot_y -= 0.05f;
	else if (key == KEY_D)
		fdf->cam->rot_y += 0.05f;
}

static void	handle_view(int key, t_fdf *fdf)
{
	if (key == KEY_PLUS)
		fdf->cam->zoom *= 1.1f;
	else if (key == KEY_MINUS)
		fdf->cam->zoom /= 1.1f;
	else if (key == KEY_Z_UP)
		fdf->cam->z_scale *= 1.2f;
	else if (key == KEY_Z_DOWN)
		fdf->cam->z_scale /= 1.2f;
	else if (key == KEY_P)
		fdf->cam->proj = !fdf->cam->proj;
	else if (key == KEY_C)
		fdf->flags = (fdf->flags & ~FDF_THEME_MASK)
			| (((fdf_theme(fdf) + 1) % 3) << FDF_THEME_SHIFT);
	else if (key == KEY_SPACE)
		fdf->flags ^= FDF_AUTO_ROTATE;
	else if (key == KEY_F12)
		save_screenshot(fdf);
	else
		handle_rotation(key, fdf);
}

static t_preset	pick_preset(int id)
{
	if (id == 1)
		return (preset_island());
	if (id == 2)
		return (preset_desert());
	if (id == 3)
		return (preset_forest());
	if (id == 4)
		return (preset_fjords());
	return (preset_mountain());
}

/* z_max suit la taille : sans ça une map 1000x1000 paraît plate. */
static void	handle_mapgen(int key, t_fdf *fdf)
{
	static int	sizes[5] = {100, 200, 300, 500, 1000};
	t_preset	cfg;

	if (key == KEY_G)
		fdf->seed = rand();
	else if (key == KEY_N)
		fdf->seed++;
	else if (key == KEY_TAB)
		fdf->flags = (fdf->flags & ~FDF_SIZE_MASK)
			| (((fdf_size(fdf) + 1) % 5) << FDF_SIZE_SHIFT);
	else
		fdf->flags = (fdf->flags & ~FDF_PRESET_MASK)
			| ((key - KEY_1) << FDF_PRESET_SHIFT);
	cfg = pick_preset(fdf_preset(fdf));
	cfg.width = sizes[fdf_size(fdf)];
	cfg.height = sizes[fdf_size(fdf)];
	cfg.z_max = cfg.z_max * sizes[fdf_size(fdf)] / 250;
	free_map(fdf->map);
	generate_fdf_file("/tmp/fdf_gen.fdf", fdf->seed, &cfg);
	fdf->map = parse_map("/tmp/fdf_gen.fdf");
	init_cam(fdf);
}

int	key_hook(int key, void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	if (key == KEY_ESC)
		quit(fdf);
	else if (key == KEY_R)
		init_cam(fdf);
	else if (key == KEY_UP)
		fdf->cam->y_off -= 20;
	else if (key == KEY_DOWN)
		fdf->cam->y_off += 20;
	else if (key == KEY_LEFT)
		fdf->cam->x_off -= 20;
	else if (key == KEY_RIGHT)
		fdf->cam->x_off += 20;
	else if (key == KEY_G || key == KEY_N || key == KEY_TAB
		|| (key >= KEY_1 && key <= KEY_5))
		handle_mapgen(key, fdf);
	else
		handle_view(key, fdf);
	fdf->flags |= FDF_DIRTY;
	return (0);
}
