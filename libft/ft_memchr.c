/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:19:34 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 10:53:45 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return ((unsigned char *)s + i);
		i++;
	}
	return (NULL);
}
/*
int	main(void) {
   const char str[] = "Welcome to Turkey";
   const char ch = 't';

   char* result = (char*)ft_memchr(str, ch, strlen(str));

   if (result != NULL) {
		printf("'%c' found at position %ld\n", ch, result - str);
   } else {
		printf("'%c' not found in the string\n", ch);
   }

   return (0);
}
*/