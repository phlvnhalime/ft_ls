/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:57:02 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 17:23:29 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack && !len)
		return (0);
	if (needle[0] == '\0' || needle == haystack)
		return ((char *)haystack);
	while (haystack[i] != '\0' && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && (i + j) < len)
		{
			if (haystack[i + j] == '\0' && needle[j] == '\0')
				return ((char *)&haystack[i]);
			j++;
		}
		if (needle[j] == '\0')
			return ((char *)(haystack + i));
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	const char	*haystack = "Hello, welcome to the world of programming!";
	const char	*needle = "welcome";
	size_t		len;
	char		*result;

	len = 30;
	result = ft_strnstr(haystack, needle, len);
	if (result)
		printf("Found '%s' in '%s' within the first %zu characters: %s\n",
			needle, haystack, len, result);
	else
		printf("'%s' not found in the first %zu characters of '%s'.\n", needle,
			len, haystack);
	return (0);
}
*/