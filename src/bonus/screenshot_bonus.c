/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:55:25 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/16 16:27:25 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	write_bmp_header(int fd)
{
	static const unsigned char	h[54] = {
		'B', 'M',
		0x36, 0xEC, 0x5E, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x36, 0x00, 0x00, 0x00,
		0x28, 0x00, 0x00, 0x00,
		0x80, 0x07, 0x00, 0x00,
		0x38, 0x04, 0x00, 0x00,
		0x01, 0x00,
		0x18, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x13, 0x0B, 0x00, 0x00,
		0x13, 0x0B, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	write(fd, h, 54);
}

static void	write_row(int fd, t_fdf *fdf, int y)
{
	unsigned char	row[WIN_W * 3];
	unsigned int	color;
	int				x;

	x = 0;
	while (x < WIN_W)
	{
		color = *(unsigned int *)(fdf->addr
				+ y * fdf->line_len + x * (fdf->bpp / 8));
		row[x * 3] = color & 0xFF;
		row[x * 3 + 1] = (color >> 8) & 0xFF;
		row[x * 3 + 2] = (color >> 16) & 0xFF;
		x++;
	}
	write(fd, row, WIN_W * 3);
}

void	save_screenshot(t_fdf *fdf)
{
	int	fd;
	int	y;

	fd = open("screenshot.bmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	write_bmp_header(fd);
	y = WIN_H - 1;
	while (y >= 0)
	{
		write_row(fd, fdf, y);
		y--;
	}
	close(fd);
}
