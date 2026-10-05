/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:53:32 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 18:12:46 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	size;
	char	*dst;

	size = ft_strlen(s1);
	dst = (char *)malloc((size + 1) * sizeof(char));
	if (dst == NULL)
		return (0);
	ft_memcpy(dst, s1, size);
	dst[size] = '\0';
	return (dst);
}

/*
int	main(void) {
	const char *original = "Hello, world!";
	char *duplicate = ft_strdup(original);
	if (duplicate != NULL) {
		printf("Original string: %s\n", original);
		printf("Duplicate string: %s\n", duplicate);
		free(duplicate); // Release the dynamically allocated memory
	} else {
		printf("Memory allocation failed.\n");
	}
	return (0);
}

*/
