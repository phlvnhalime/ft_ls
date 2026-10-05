/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:16:36 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 18:37:43 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	s_len;
	size_t	s_last;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	s_last = s_len - start;
	if (s_last > len)
		s_last = len;
	new = (char *)malloc(sizeof(char) * (s_last + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, s + start, s_last + 1);
	return (new);
}
