/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:53:53 by ntassin           #+#    #+#             */
/*   Updated: 2025/11/12 10:02:40 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *d, const char *s, size_t dstsize)
{
	size_t	dlen;
	size_t	slen;
	size_t	i;

	dlen = ft_strlen(d);
	slen = ft_strlen(s);
	if (dstsize <= dlen)
		return (dstsize + slen);
	i = 0;
	while (s[i] && dlen + i < dstsize - 1)
	{
		d[dlen + i] = s[i];
		i++;
	}
	d[dlen + i] = '\0';
	return (dlen + slen);
}
