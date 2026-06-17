/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fbm_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mapgen_bonus.h"

/*
** Bruit fractal : N octaves superposées → retourne [0.0, 1.0].
** L'offset i×19.19/i×47.47 décorrèle les octaves : sans lui, leurs
** maxima s'alignent sur la même grille → artefacts visibles.
*/
double	fbm(t_perlin *pn, double x, double y, t_preset *cfg)
{
	double	value;
	double	amplitude;
	double	frequency;
	double	max_val;
	int		i;

	value = 0.0;
	amplitude = 1.0;
	frequency = cfg->scale;
	max_val = 0.0;
	i = 0;
	while (i < cfg->octaves)
	{
		value += noise2d(pn, x * frequency + i * 19.19,
				y * frequency + i * 47.47) * amplitude;
		max_val += amplitude;
		amplitude *= cfg->persistence;
		frequency *= cfg->lacunarity;
		i++;
	}
	return ((value / max_val) * 0.5 + 0.5);
}

/*
** Bruit "ridged" : 1 - |bruit| transforme les passages par zéro en
** crêtes vives → arêtes montagneuses réalistes au lieu de bosses rondes.
** Le carré (n × n) creuse les vallées. Normalisation : somme géométrique
** des amplitudes = (1 - p^octaves) / (1 - p).
*/
static double	ridged_fbm(t_perlin *pn, double x, double y, t_preset *cfg)
{
	double	value;
	double	amplitude;
	double	frequency;
	double	n;
	int		i;

	value = 0.0;
	amplitude = 1.0;
	frequency = cfg->scale;
	i = -1;
	while (++i < cfg->octaves)
	{
		n = 1.0 - fabs(noise2d(pn, x * frequency + i * 19.19,
					y * frequency + i * 47.47)) * 1.5;
		if (n < 0.0)
			n = 0.0;
		value += n * n * amplitude;
		amplitude *= cfg->persistence;
		frequency *= cfg->lacunarity;
	}
	return (value * (1.0 - cfg->persistence)
		/ (1.0 - pow(cfg->persistence, cfg->octaves)));
}

/* Sélectionne le bruit du terrain : ridged (crêtes) ou fBm classique. */
double	height_at(t_perlin *pn, double x, double y, t_preset *cfg)
{
	if (cfg->is_ridged)
		return (ridged_fbm(pn, x, y, cfg));
	return (fbm(pn, x, y, cfg));
}

/*
** Tord les coordonnées avec deux fBm indépendants avant de lire le terrain.
** Les offsets 5.2/1.3 évitent la symétrie entre warp_x et warp_y.
*/
double	domain_warp(t_perlin *pn, double x, double y, t_preset *cfg)
{
	double	warp_x;
	double	warp_y;

	warp_x = fbm(pn, x, y, cfg);
	warp_y = fbm(pn, x + 5.2, y + 1.3, cfg);
	return (height_at(pn,
			x + warp_x * cfg->warp_force,
			y + warp_y * cfg->warp_force,
			cfg));
}

/*
** Gradient radial : force les bords à descendre sous l'eau.
** mask : +1 au centre, 0 à mi-distance, -1 au bord → le centre émerge,
** le pourtour plonge sous SEA_LEVEL quel que soit le bruit.
*/
double	apply_island_mask(double h, double x, double y, t_preset *cfg)
{
	double	nx;
	double	ny;
	double	dist;
	double	mask;

	nx = x / (double)(cfg->width - 1) * 2.0 - 1.0;
	ny = y / (double)(cfg->height - 1) * 2.0 - 1.0;
	dist = sqrt(nx * nx + ny * ny) / sqrt(2.0);
	mask = 1.0 - (dist * dist * 2.0);
	h = h + mask * 0.45 - 0.12;
	if (h < 0.0)
		return (0.0);
	return (h);
}
