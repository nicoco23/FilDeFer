/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:11:19 by ntassin           #+#    #+#             */
/*   Updated: 2026/04/27 13:36:11 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_stash(int fd, char *stash)
{
	char	*buf;
	char	*temp;
	ssize_t	bytes;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	bytes = 1;
	while (bytes > 0 && !ft_strchr(stash, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buf), free(stash), NULL);
		buf[bytes] = '\0';
		temp = stash;
		stash = ft_strjoin(stash, buf);
		free(temp);
		if (!stash)
			return (free(buf), NULL);
	}
	return (free(buf), stash);
}

static char	*extract_line(char *stash)
{
	size_t	len;
	char	*nl;

	nl = ft_strchr(stash, '\n');
	if (nl)
		len = (size_t)(nl - stash) + 1;
	else
		len = ft_strlen(stash);
	return (ft_substr(stash, 0, len));
}

static char	*get_new_stash(char *stash, char *line)
{
	char	*new_stash;
	size_t	len;

	len = ft_strlen(line);
	new_stash = ft_substr(stash, len, ft_strlen(stash) - len);
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash[fd])
	{
		stash[fd] = ft_strjoin("", "");
		if (!stash[fd])
			return (NULL);
	}
	stash[fd] = read_and_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	if (stash[fd][0] == '\0')
		return (free(stash[fd]), stash[fd] = NULL, NULL);
	line = extract_line(stash[fd]);
	stash[fd] = get_new_stash(stash[fd], line);
	return (line);
}
