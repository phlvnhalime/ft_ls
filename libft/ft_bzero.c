/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:47:57 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/15 15:50:39 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	size_t	i;
	char	*str;

	i = 0;
	str = s;
	while (i < len)
	{
		str[i] = 0;
		i++;
	}
}
