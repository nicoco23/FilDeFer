/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_bonus.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/17 09:29:41 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_BONUS_H
# define FDF_BONUS_H

# include <math.h>
# include <stdlib.h>
# include <time.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define WIN_W    1920
# define WIN_H    1080

# define NO_COLOR -1

/* Touches clavier (keycodes X11/Linux) */
# define KEY_ESC      65307
# define KEY_UP       65362
# define KEY_DOWN     65364
# define KEY_LEFT     65361
# define KEY_RIGHT    65363
# define KEY_PLUS     43    /* '+' */
# define KEY_MINUS    45    /* '-' */
# define KEY_Z_UP     91    /* '[' → z_scale haut */
# define KEY_Z_DOWN   93    /* ']' → z_scale bas */
# define KEY_P        112
# define KEY_Q        113
# define KEY_E        101
# define KEY_R        114
# define KEY_W        119
# define KEY_S        115
# define KEY_A        97
# define KEY_D        100
# define KEY_C        99
# define KEY_SPACE    32

/* Screenshot */
# define KEY_F12      65481

/* Génération procédurale */
# define KEY_TAB      65289
# define KEY_G        103
# define KEY_N        110
# define KEY_1        49
# define KEY_2        50
# define KEY_3        51
# define KEY_4        52
# define KEY_5        53

/* Boutons souris */
# define MOUSE_LEFT   1
# define MOUSE_RIGHT  3
# define MOUSE_UP     4
# define MOUSE_DOWN   5

/* Masques événements X11 */
# define MASK_BTN_PRESS    4
# define MASK_BTN_RELEASE  8
# define MASK_BTN_MOTION   8192

/* Numéros d'événements X11 */
# define EVT_BTN_PRESS     4
# define EVT_BTN_RELEASE   5
# define EVT_MOTION        6

/* Événement fermeture fenêtre */
# define MLX_CLOSE_EVENT  17

/* Bitmasks pour t_fdf.flags */
# define FDF_DIRTY        0x001
# define FDF_AUTO_ROTATE  0x002
# define FDF_DRAGGING     0x004
# define FDF_ROTATING     0x008
# define FDF_THEME_MASK   0x030
# define FDF_THEME_SHIFT  4
# define FDF_PRESET_MASK  0x1C0
# define FDF_PRESET_SHIFT 6
# define FDF_SIZE_MASK    0xE00
# define FDF_SIZE_SHIFT   9

typedef struct timeval	t_tv;

typedef struct s_map
{
	int		**z;
	int		**color;
	int		width;
	int		height;
	int		z_min;
	int		z_max;
}	t_map;

typedef struct s_cam
{
	float	zoom;
	int		x_off;
	int		y_off;
	float	z_scale;
	float	rot_x;
	float	rot_y;
	float	rot_z;
	int		proj;
	float	cx;
	float	sx;
	float	cy;
	float	sy;
	float	cz;
	float	sz;
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
	int		flags;
	int		seed;
	int		drag_x;
	int		drag_y;
	t_tv	last_tick;
	float	fps;
/* draw3_bonus.c */
}	t_fdf;

typedef struct s_pt
{
	int	x;
	int	y;
	int	color;
}	t_pt;

/*
** t_vec3 : point de la grille 3D (col, row, z).
** Utilisé comme paramètre unique dans les fonctions de projection
** pour rester dans la limite de 4 paramètres Norme.
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

/* parse_bonus.c */
t_map		*parse_map(char *filename);
int			count_cols_and_rows(char *filename, int *width);
void		alloc_map(t_map *map);
void		parse_token(char *token, int *z, int *color);

/* projection_bonus.c */
t_pt		iso_project(t_vec3 pt, t_cam *cam);
t_pt		parallel_project(t_vec3 pt, t_cam *cam);
void		init_cam(t_fdf *fdf);
void		update_trig(t_cam *cam);

/* draw_bonus.c */
void		draw_line(t_fdf *fdf, t_pt p1, t_pt p2);

/* draw2_bonus.c */
t_pt		map_to_screen(t_fdf *fdf, int col, int row);
void		draw_hud(t_fdf *fdf);

/* draw3_bonus.c */
void		draw_map(t_fdf *fdf);

/* events_bonus.c */
int			key_hook(int key, void *param);
int			close_hook(void *param);
void		quit(t_fdf *fdf);
void		redraw(t_fdf *fdf);
int			loop_hook(void *param);

/* mouse_bonus.c */
int			mouse_hook(int btn, int x, int y, void *param);
int			release_hook(int btn, int x, int y, void *param);
int			motion_hook(int x, int y, void *param);

/* screenshot_bonus.c */
void		save_screenshot(t_fdf *fdf);

/* utils_bonus.c */
void		free_map(t_map *map);
void		error_exit(char *msg);
int			lerp_color(int c1, int c2, float t);
int			ft_abs(int n);
int			rainbow_color(float norm);
int			fdf_theme(t_fdf *fdf);
int			fdf_preset(t_fdf *fdf);
int			fdf_size(t_fdf *fdf);

#endif
