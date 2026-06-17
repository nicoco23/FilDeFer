/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 00:00:00 by ntassin           #+#    #+#             */
/*   Updated: 2026/06/12 00:00:00 by ntassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long n)
{
	int	d;

	d = 1;
	while (n >= 10 || n <= -10)
	{
		n /= 10;
		d++;
	}
	return (d);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		neg;
	int		len;
	char	*str;

	nb = (long)n;
	neg = (nb < 0);
	if (neg)
		nb = -nb;
	len = count_digits(nb) + neg;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len-- > neg)
	{
		str[len] = (char)((nb % 10) + '0');
		nb /= 10;
	}
	if (neg)
		str[0] = '-';
	return (str);
}
