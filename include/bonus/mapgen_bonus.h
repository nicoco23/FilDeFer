/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:35:09 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/15 15:30:39 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPGEN_BONUS_H
# define MAPGEN_BONUS_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

# define SEA_LEVEL 0.38

typedef struct s_perlin
{
	int	p[512];
	int	seed;
}	t_perlin;

typedef struct s_biome_color
{
	int	low;
	int	high;
}	t_biome_color;

typedef struct s_cell
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_cell;

typedef struct s_preset
{
	int		width;
	int		height;
	int		z_max;
	int		octaves;
	int		is_island;
	int		is_ridged;
	double	persistence;
	double	lacunarity;
	double	scale;
	double	exponent;
	double	warp_force;
}	t_preset;

typedef enum e_biome
{
	BIO_DEEP_OCEAN,
	BIO_OCEAN,
	BIO_BEACH,
	BIO_TUNDRA,
	BIO_TAIGA,
	BIO_PLAINS,
	BIO_SAVANNA,
	BIO_FOREST,
	BIO_DESERT,
	BIO_MOUNTAIN,
	BIO_HIGH_MOUNTAIN,
	BIO_SNOW
}	t_biome;

/* perlin_bonus.c */
void		perlin_init(t_perlin *pn, int seed);
double		noise2d(t_perlin *pn, double x, double y);

/* fbm_bonus.c */
double		fbm(t_perlin *pn, double x, double y, t_preset *cfg);
double		height_at(t_perlin *pn, double x, double y, t_preset *cfg);
double		domain_warp(t_perlin *pn, double x, double y, t_preset *cfg);
double		apply_island_mask(double h, double x, double y, t_preset *cfg);

/* biome_bonus.c */
double		compute_temp(t_perlin *pn, t_cell *cell, t_preset *cfg, double h);
t_biome		get_biome(double height, double temp);
int			biome_color(t_biome biome, double t_norm);
int			compute_z(double h, int z_max);

/* mapgen_write_bonus.c */
void		generate_fdf_file(char *filename, int seed, t_preset *cfg);

/* mapgen_bonus.c */
t_preset	preset_mountain(void);
t_preset	preset_island(void);
t_preset	preset_desert(void);
t_preset	preset_forest(void);
t_preset	preset_fjords(void);

#endif
