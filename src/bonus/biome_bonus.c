/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   biome_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mapgen_bonus.h"

/*
** Température = latitude + altitude + dérive bruitée.
** Sans le bruit, les frontières de biomes suivent exactement les
** lignes horizontales (latitude) → bandes artificielles.
*/
double	compute_temp(t_perlin *pn, t_cell *cell, t_preset *cfg, double h)
{
	double	lat;
	double	drift;

	lat = 1.0 - (double)cell->y / (double)(cfg->height - 1);
	drift = noise2d(pn, cell->x * 0.013 + 311.7, cell->y * 0.013 + 157.3);
	return (lat - h * 0.4 + drift * 0.25);
}

static t_biome	get_vegetation_biome(double temp, double h)
{
	if (temp < 0.18)
	{
		if (h > 0.57)
			return (BIO_TAIGA);
		return (BIO_TUNDRA);
	}
	if (temp > 0.70)
		return (BIO_DESERT);
	if (temp > 0.50)
	{
		if (h > 0.57)
			return (BIO_FOREST);
		return (BIO_SAVANNA);
	}
	if (h > 0.57)
		return (BIO_FOREST);
	return (BIO_PLAINS);
}

t_biome	get_biome(double height, double temp)
{
	if (height < 0.28)
		return (BIO_DEEP_OCEAN);
	if (height < 0.38)
		return (BIO_OCEAN);
	if (height < 0.43)
		return (BIO_BEACH);
	if (height > 0.94)
		return (BIO_SNOW);
	if (height > 0.86)
		return (BIO_HIGH_MOUNTAIN);
	if (height > 0.72)
		return (BIO_MOUNTAIN);
	return (get_vegetation_biome(temp, height));
}

int	biome_color(t_biome biome, double t_norm)
{
	static int	lo[12] = {0x03143C, 0x0D3878, 0xD4B87A,
		0x6A7A62, 0x1A3E18, 0x4EA830, 0x96A424,
		0x1C5020, 0xC07830, 0x64584A,
		0x8C7C6C, 0xD8E8F4};
	static int	hi[12] = {0x0D3878, 0x1E5FA8, 0xECD89E,
		0x9AAC8A, 0x2C5E24, 0x78C84A, 0xBCC840,
		0x2E7834, 0xDCAA60, 0x8C7C6C,
		11839640, 16777215};
	int			r;
	int			g;
	int			b;

	r = (int)(((lo[biome] >> 16) & 255) * (1.0 - t_norm)
			+ ((hi[biome] >> 16) & 255) * t_norm);
	g = (int)(((lo[biome] >> 8) & 255) * (1.0 - t_norm)
			+ ((hi[biome] >> 8) & 255) * t_norm);
	b = (int)((lo[biome] & 255) * (1.0 - t_norm)
			+ (hi[biome] & 255) * t_norm);
	return ((r << 16) | (g << 8) | b);
}

int	compute_z(double h, int z_max)
{
	if (h < SEA_LEVEL)
		return ((int)(h / SEA_LEVEL * -5.0));
	return ((int)((h - SEA_LEVEL) / (1.0 - SEA_LEVEL) * (double)z_max));
}
