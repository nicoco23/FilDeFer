/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/05/04 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static int	count_width(char *line)
{
	char	**tokens;
	int		width;

	tokens = ft_split(line, ' ');
	width = 0;
	while (tokens && tokens[width])
	{
		free(tokens[width]);
		width++;
	}
	free(tokens);
	return (width);
}

int	count_cols_and_rows(char *filename, int *width)
{
	int		fd;
	char	*line;
	int		count;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Error: Cannot open file");
	count = 0;
	*width = 0;
	line = get_next_line(fd);
	if (line)
		*width = count_width(line);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

void	alloc_map(t_map *map)
{
	int	i;

	map->z = (int **)malloc(sizeof(int *) * map->height);
	map->color = (int **)malloc(sizeof(int *) * map->height);
	if (!map->z || !map->color)
		error_exit("Error: Malloc failed");
	i = 0;
	while (i < map->height)
	{
		map->z[i] = (int *)malloc(sizeof(int) * map->width);
		map->color[i] = (int *)malloc(sizeof(int) * map->width);
		if (!map->z[i] || !map->color[i])
			error_exit("Error: malloc failed");
		i++;
	}
}

static int	parse_hex_color(char *str)
{
	int	color;
	int	i;

	color = 0;
	i = 1;
	if (str[1] == '0' && (str[2] == 'x' || str[2] == 'X'))
		i = 3;
	while (str[i] && str[i] != '\n')
	{
		color *= 16;
		if (str[i] >= '0' && str[i] <= '9')
			color += str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			color += str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			color += str[i] - 'A' + 10;
		i++;
	}
	return (color);
}

void	parse_token(char *token, int *z, int *color)
{
	char	*comma;

	*color = NO_COLOR;
	comma = ft_strchr(token, ',');
	if (comma)
	{
		*comma = '\0';
		*z = ft_atoi(token);
		*color = parse_hex_color(comma);
	}
	else
		*z = ft_atoi(token);
}
