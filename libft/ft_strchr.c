/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:43:30 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 16:49:04 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while ((char)c != *s)
	{
		if (!*s)
			return (0);
		s++;
	}
	return ((char *)s);
}
/*
int	main(void)
{
   const char *str = "Welcome to Tutorialspoint";
	char	ch;
	char	*p;

   ch = 'u';
   // Find the first occurrence of 'u' in the string
   p = ft_strchr(str, ch);
   if (p != NULL)
   {
		printf("String starting from '%c' is: %s\n", ch, p);
   }
   else
   {
		printf("Character '%c' not found in the string.\n", ch);
   }
   return (0);
}
*/
/*
int main ()
{
   const char str[] = "Tutorialspoint";
   const char ch = '.';
   char *ret;

   // "ch" is search string
   ret = ft_strchr(str, ch);
   printf("String after |%c| is - |%s|\n", ch, ret);
   return(0);
}
*/
