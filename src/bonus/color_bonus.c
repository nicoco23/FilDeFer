/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

int	rainbow_color(float norm)
{
	static int	colors[7] = {
		0xFF0000, 0xFF7F00, 0xFFFF00,
		0x00FF00, 0x0000FF, 0x4B0082, 0xFF0000
	};
	float		h;
	int			i;

	h = norm * 6.0f;
	i = (int)h;
	if (i >= 6)
		i = 5;
	return (lerp_color(colors[i], colors[i + 1], h - i));
}

int	fdf_theme(t_fdf *fdf)
{
	return ((fdf->flags & FDF_THEME_MASK) >> FDF_THEME_SHIFT);
}

int	fdf_preset(t_fdf *fdf)
{
	return ((fdf->flags & FDF_PRESET_MASK) >> FDF_PRESET_SHIFT);
}

int	fdf_size(t_fdf *fdf)
{
	return ((fdf->flags & FDF_SIZE_MASK) >> FDF_SIZE_SHIFT);
}
