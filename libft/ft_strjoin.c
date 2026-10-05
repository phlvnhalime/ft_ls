/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:05:21 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/17 12:14:44 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tstr;
	int		count[2];

	count[0] = 0;
	count[1] = 0;
	if (!s1 || !s2)
		return (NULL);
	tstr = (char *)malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!tstr)
		return (NULL);
	while (s1[count[0]])
	{
		tstr[count[0]] = s1[count[0]];
		count[0]++;
	}
	while (s2[count[1]])
	{
		tstr[count[0]] = s2[count[1]];
		count[0]++;
		count[1]++;
	}
	tstr[count[0]] = '\0';
	return (tstr);
}
