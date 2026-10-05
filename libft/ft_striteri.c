/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:01:26 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/16 11:38:25 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
	s[i] = '\0';
}
/*
int	ft_toupper(int	c)
{
	if (c >= 97 && c <= 122)
		c -= 32;
	return (c);
}


void	apply_toupper(unsigned int i, char *c)
{
	*c = ft_toupper(*c);
}

int	main(void)
{
	char	message[] = "Hello Words";

	printf("Orginal string: %s\n", message);
	ft_striteri(message,apply_toupper);
	printf("Modified string: %s\n" , message);
	return (0);
}
*/
