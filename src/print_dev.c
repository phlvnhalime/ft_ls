/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:29 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static unsigned int	dev_major(dev_t dev)
{
	return (((dev >> 8) & 0xfff)
		| ((unsigned int)(dev >> 32) & ~0xfffu));
}

static unsigned int	dev_minor(dev_t dev)
{
	return ((dev & 0xff)
		| ((unsigned int)(dev >> 12) & ~0xffu));
}

void	fill_size(struct stat *st, char *buf)
{
	char	minor_buf[32];
	int		i;
	int		j;

	if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode))
	{
		ull_to_str((unsigned long long)dev_major(st->st_rdev), buf);
		i = 0;
		while (buf[i])
			i++;
		buf[i++] = ',';
		buf[i++] = ' ';
		ull_to_str((unsigned long long)dev_minor(st->st_rdev), minor_buf);
		j = 0;
		while (minor_buf[j])
			buf[i++] = minor_buf[j++];
		buf[i] = '\0';
		return ;
	}
	ull_to_str((unsigned long long)st->st_size, buf);
}

long long	block_units(struct stat *st)
{
	return ((long long)st->st_blocks / 2);
}
