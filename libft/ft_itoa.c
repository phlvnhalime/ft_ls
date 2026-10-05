/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:28:34 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/17 16:37:40 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_digit_nb(int i)
{
	int	len;

	len = 0;
	if (i <= 0)
		len = 1;
	while (i)
	{
		i /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	len;
	long	nb;
	char	*result;

	len = ft_digit_nb(n);
	nb = n;
	if (n < 0)
		nb *= -1;
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = 0;
	if (nb == 0)
		result[0] = '0';
	else
	{
		while (len--, nb != 0)
		{
			result[len] = nb % 10 + '0';
			nb = (nb - (nb % 10)) / 10;
		}
		if (n < 0)
			result[len] = '-';
	}
	return (result);
}
