/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:55:30 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/15 17:17:57 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	char			x;
	unsigned char	*cpy;
	unsigned char	*str;

	i = 0;
	x = 1;
	cpy = (unsigned char *)dst;
	str = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	if (dst > src)
	{
		x = -1;
		cpy += len - 1;
		str += len - 1;
	}
	while (i < len)
	{
		*cpy = *str;
		cpy += x;
		str += x;
		i++;
	}
	return (dst);
}
