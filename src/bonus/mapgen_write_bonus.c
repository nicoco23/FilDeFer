/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen_write_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mapgen_bonus.h"

/* Écrit un entier signé dans buf — sans malloc ni printf. */
static void	append_int(char *buf, int *idx, int n)
{
	char	rev[12];
	int		r_i;

	r_i = 0;
	if (n < 0)
	{
		buf[(*idx)++] = '-';
		n = -n;
	}
	if (n == 0)
		buf[(*idx)++] = '0';
	while (n > 0)
	{
		rev[r_i++] = (n % 10) + '0';
		n /= 10;
	}
	while (r_i > 0)
		buf[(*idx)++] = rev[--r_i];
}

/* Écrit ",0xRRGGBB" dans buf — sans malloc ni printf. */
static void	append_hex(char *buf, int *idx, int color)
{
	static char	*hex = "0123456789ABCDEF";
	char		rev[6];
	int			r_i;

	r_i = 0;
	while (r_i < 6)
	{
		rev[r_i] = hex[color & 0xF];
		color >>= 4;
		r_i++;
	}
	buf[(*idx)++] = ',';
	buf[(*idx)++] = '0';
	buf[(*idx)++] = 'x';
	while (r_i > 0)
		buf[(*idx)++] = rev[--r_i];
}

/*
** Pipeline complète d'une cellule : bruit → masque → redistribution
** → température → biome → z et couleur.
*/
static void	compute_cell(t_perlin *pn, t_preset *cfg, t_cell *cell)
{
	double	h;
	double	h_n;
	double	temp;
	t_biome	biome;

	if (cfg->warp_force > 0.0)
		h = domain_warp(pn, (double)cell->x, (double)cell->y, cfg);
	else
		h = height_at(pn, (double)cell->x, (double)cell->y, cfg);
	if (cfg->is_island)
		h = apply_island_mask(h, (double)cell->x, (double)cell->y, cfg);
	h = pow(h, cfg->exponent);
	if (h >= 1.0)
		h_n = 1.0;
	else
		h_n = h;
	temp = compute_temp(pn, cell, cfg, h_n);
	biome = get_biome(h_n, temp);
	cell->color = biome_color(biome, h_n);
	cell->z = compute_z(h, cfg->z_max);
}

/* Construit une ligne entière en RAM, puis 1 seul write() (Fast I/O). */
static void	write_row(int fd, t_perlin *pn, t_preset *cfg, int y)
{
	char	buf[65536];
	t_cell	cell;
	int		idx;

	idx = 0;
	cell.y = y;
	cell.x = 0;
	while (cell.x < cfg->width)
	{
		compute_cell(pn, cfg, &cell);
		append_int(buf, &idx, cell.z);
		append_hex(buf, &idx, cell.color);
		if (cell.x + 1 < cfg->width)
			buf[idx++] = ' ';
		cell.x++;
	}
	buf[idx++] = '\n';
	write(fd, buf, idx);
}

void	generate_fdf_file(char *filename, int seed, t_preset *cfg)
{
	t_perlin	pn;
	int			fd;
	int			y;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	perlin_init(&pn, seed);
	y = 0;
	while (y < cfg->height)
	{
		write_row(fd, &pn, cfg, y);
		y++;
	}
	close(fd);
}
