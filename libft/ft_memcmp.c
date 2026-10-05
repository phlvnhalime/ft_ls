/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:04:24 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/15 14:17:48 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str;
	unsigned char	*str1;

	if (n == 0)
		return (0);
	str = (unsigned char *)s1;
	str1 = (unsigned char *)s2;
	while ((*str == *str1) && (n - 1 > 0))
	{
		str++;
		str1++;
		n--;
	}
	return ((int)(*str - *str1));
}
