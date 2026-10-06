/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_num.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:32 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	ull_to_str(unsigned long long n, char *buf)
{
	char	tmp[32];
	int		i;
	int		j;

	i = 0;
	if (n == 0)
		tmp[i++] = '0';
	while (n > 0)
	{
		tmp[i++] = (char)('0' + (n % 10));
		n /= 10;
	}
	j = 0;
	while (i > 0)
		buf[j++] = tmp[--i];
	buf[j] = '\0';
}

int	ull_len(unsigned long long n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	put_ull(unsigned long long n)
{
	char	buf[32];

	ull_to_str(n, buf);
	ft_putstr_fd(buf, 1);
}

void	pad_spaces(int n)
{
	while (n > 0)
	{
		ft_putchar_fd(' ', 1);
		n--;
	}
}

void	widen(int *width, int len)
{
	if (len > *width)
		*width = len;
}
