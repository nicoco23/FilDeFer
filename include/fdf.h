/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:22:11 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/06 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <math.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define WIN_W            1920
# define WIN_H            1080

# define NO_COLOR         -1

/* Touche clavier (keycode X11/Linux) */
# define KEY_ESC          65307

/* Événement fermeture fenêtre (X11/MLX) */
# define MLX_CLOSE_EVENT  17

typedef struct s_map
{
	int	**z;
	int	**color;
	int	width;
	int	height;
	int	z_min;
	int	z_max;
}	t_map;

typedef struct s_cam
{
	float	zoom;
	int		x_off;
	int		y_off;
	float	z_scale;
}	t_cam;

typedef struct s_fdf
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	t_map	*map;
	t_cam	*cam;
}	t_fdf;

typedef struct s_pt
{
	int	x;
	int	y;
	int	color;
}	t_pt;

/*
** t_vec3 : point de la grille 3D (col, row, z).
** Passé par valeur dans iso_project pour rester ≤ 4 paramètres (Norme).
*/
typedef struct s_vec3
{
	int	col;
	int	row;
	int	z;
}	t_vec3;

typedef struct s_line
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
}	t_line;

/* parse.c */
t_map	*parse_map(char *filename);
int		count_cols_and_rows(char *filename, int *width);
void	alloc_map(t_map *map);
void	parse_token(char *token, int *z, int *color);

/* projection.c */
t_pt	iso_project(t_vec3 pt, t_cam *cam);
void	init_cam(t_fdf *fdf);

/* draw.c / draw2.c */
void	draw_map(t_fdf *fdf);
void	draw_line(t_fdf *fdf, t_pt p1, t_pt p2);
t_pt	map_to_screen(t_fdf *fdf, int col, int row);

/* events.c */
int		key_hook(int key, void *param);
int		close_hook(void *param);
void	quit(t_fdf *fdf);

/* utils.c */
void	free_map(t_map *map);
void	error_exit(char *msg);
int		lerp_color(int c1, int c2, float t);
int		ft_abs(int n);

#endif
