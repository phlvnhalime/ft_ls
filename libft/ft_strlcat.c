/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:50:21 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 15:02:38 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	total_len;
	size_t	cpy_len;

	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	total_len = dest_len + src_len;
	cpy_len = dstsize - 1 - dest_len;
	if (dstsize <= dest_len)
		return (dstsize + src_len);
	while (*dst != '\0')
		dst++;
	while (*src != '\0' && cpy_len > 0)
	{
		*dst = *src;
		dst++;
		src++;
		cpy_len--;
	}
	*dst = '\0';
	return (total_len);
}
