/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:34:25 by ntassin           #+#    #+#             */
/*   Updated: 2026/04/27 18:57:24 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	update_minmax(t_map *map, int r, int c)
{
	if (map->z[r][c] < map->z_min)
		map->z_min = map->z[r][c];
	if (map->z[r][c] > map->z_max)
		map->z_max = map->z[r][c];
}

static void	parse_row(char **tokens, t_map *map, int r)
{
	int	c;

	c = 0;
	while (c < map->width && tokens && tokens[c])
	{
		parse_token(tokens[c], &map->z[r][c], &map->color[r][c]);
		update_minmax(map, r, c);
		free(tokens[c]);
		c++;
	}
	while (tokens && tokens[c])
		free(tokens[c++]);
	free(tokens);
}

static void	fill_map(int fd, t_map *map)
{
	char	*line;
	char	**tokens;
	int		r;

	r = 0;
	map->z_min = INT_MAX;
	map->z_max = INT_MIN;
	line = get_next_line(fd);
	while (line)
	{
		tokens = ft_split(line, ' ');
		parse_row(tokens, map, r);
		free(line);
		line = get_next_line(fd);
		r++;
	}
}

t_map	*parse_map(char *filename)
{
	t_map	*map;
	int		fd;

	map = (t_map *)malloc(sizeof(t_map));
	if (!map)
		error_exit("Error: malloc failed");
	map->height = count_cols_and_rows(filename, &map->width);
	if (map->height == 0 || map->width == 0)
		error_exit("Error: invalid map");
	alloc_map(map);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Error: cannot open file");
	fill_map(fd, map);
	close(fd);
	return (map);
}
