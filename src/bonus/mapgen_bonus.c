/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mapgen_bonus.h"

/* Alpes : bruit ridged → arêtes vives, vallées creusées. */
t_preset	preset_mountain(void)
{
	t_preset	p;

	p.width = 250;
	p.height = 250;
	p.z_max = 120;
	p.octaves = 7;
	p.persistence = 0.55;
	p.lacunarity = 2.1;
	p.scale = 0.004;
	p.exponent = 0.8;
	p.warp_force = 0.0;
	p.is_island = 0;
	p.is_ridged = 1;
	return (p);
}

/* Île tropicale : masque radial, warping léger, relief modéré. */
t_preset	preset_island(void)
{
	t_preset	p;

	p.width = 250;
	p.height = 250;
	p.z_max = 65;
	p.octaves = 5;
	p.persistence = 0.52;
	p.lacunarity = 2.0;
	p.scale = 0.007;
	p.exponent = 1.3;
	p.warp_force = 1.2;
	p.is_island = 1;
	p.is_ridged = 0;
	return (p);
}

/* Dunes sinueuses : peu d'octaves, relief faible, warping moyen. */
t_preset	preset_desert(void)
{
	t_preset	p;

	p.width = 250;
	p.height = 250;
	p.z_max = 25;
	p.octaves = 3;
	p.persistence = 0.45;
	p.lacunarity = 1.8;
	p.scale = 0.006;
	p.exponent = 0.9;
	p.warp_force = 1.5;
	p.is_island = 0;
	p.is_ridged = 0;
	return (p);
}

/* Collines douces : persistence haute, beaucoup de détails de surface. */
t_preset	preset_forest(void)
{
	t_preset	p;

	p.width = 250;
	p.height = 250;
	p.z_max = 40;
	p.octaves = 4;
	p.persistence = 0.60;
	p.lacunarity = 2.0;
	p.scale = 0.005;
	p.exponent = 0.8;
	p.warp_force = 0.5;
	p.is_island = 0;
	p.is_ridged = 0;
	return (p);
}

/* Fjords nordiques : warping fort + ridged + île → côtes déchirées. */
t_preset	preset_fjords(void)
{
	t_preset	p;

	p.width = 300;
	p.height = 300;
	p.z_max = 110;
	p.octaves = 7;
	p.persistence = 0.52;
	p.lacunarity = 2.2;
	p.scale = 0.006;
	p.exponent = 1.4;
	p.warp_force = 4.5;
	p.is_island = 1;
	p.is_ridged = 1;
	return (p);
}
