/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:09:53 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 16:48:06 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	return (0);
}
/*
int	main(void)
{
	int			len;
	const char	str[] = "https://www.tutorialspoint.com";
	const char	ch = '.';
	char		*ret;

	ret = ft_strrchr(str, ch);
	printf("String after |%c| is - |%s|\n", ch, ret);
	return (0);
}
*/