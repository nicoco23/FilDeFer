/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 13:43:30 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mapgen_bonus.h"

static double	fade(double t)
{
	return (t * t * t * (t * (t * 6.0 - 15.0) + 10.0));
}

static double	grad2(int hash, double x, double y)
{
	int	h;

	h = hash & 3;
	if (h == 0)
		return (x + y);
	if (h == 1)
		return (-x + y);
	if (h == 2)
		return (x - y);
	return (-x - y);
}

static void	set_hashes(t_perlin *pn, int xi, int yi, int *h)
{
	h[0] = pn->p[pn->p[xi] + yi];
	h[1] = pn->p[pn->p[xi] + yi + 1];
	h[2] = pn->p[pn->p[xi + 1] + yi];
	h[3] = pn->p[pn->p[xi + 1] + yi + 1];
}

void	perlin_init(t_perlin *pn, int seed)
{
	unsigned int	rng;
	int				i;
	int				j;
	int				tmp;

	i = -1;
	while (++i < 256)
		pn->p[i] = i;
	rng = (unsigned int)seed;
	i = 255;
	while (i > 0)
	{
		rng = rng * 1664525u + 1013904223u;
		j = (int)(rng % (unsigned int)(i + 1));
		tmp = pn->p[i];
		pn->p[i] = pn->p[j];
		pn->p[j] = tmp;
		i--;
	}
	i = -1;
	while (++i < 256)
		pn->p[256 + i] = pn->p[i];
	pn->seed = seed;
}

double	noise2d(t_perlin *pn, double x, double y)
{
	double	xf;
	double	yf;
	double	aa;
	double	ab;
	int		h[4];

	xf = x - floor(x);
	yf = y - floor(y);
	set_hashes(pn, (int)floor(x) & 255, (int)floor(y) & 255, h);
	aa = grad2(h[0], xf, yf) + fade(xf)
		* (grad2(h[2], xf - 1, yf) - grad2(h[0], xf, yf));
	ab = grad2(h[1], xf, yf - 1) + fade(xf)
		* (grad2(h[3], xf - 1, yf - 1) - grad2(h[1], xf, yf - 1));
	return (aa + fade(yf) * (ab - aa));
}
