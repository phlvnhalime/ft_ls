/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:31 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void	fill_filetype(mode_t mode, char *buf)
{
	if (S_ISREG(mode))
		buf[0] = '-';
	else if (S_ISDIR(mode))
		buf[0] = 'd';
	else if (S_ISLNK(mode))
		buf[0] = 'l';
	else if (S_ISCHR(mode))
		buf[0] = 'c';
	else if (S_ISBLK(mode))
		buf[0] = 'b';
	else if (S_ISFIFO(mode))
		buf[0] = 'p';
	else if (S_ISSOCK(mode))
		buf[0] = 's';
	else
		buf[0] = '?';
}

static void	fill_rwx(mode_t mode, char *buf)
{
	const char	*rwx;
	int			i;

	rwx = "rwxrwxrwx";
	i = 0;
	while (i < 9)
	{
		if (mode & (1 << (8 - i)))
			buf[i + 1] = rwx[i];
		else
			buf[i + 1] = '-';
		i++;
	}
}

static void	fill_special(mode_t mode, char *buf)
{
	if (mode & S_ISUID)
	{
		if (mode & S_IXUSR)
			buf[3] = 's';
		else
			buf[3] = 'S';
	}
	if (mode & S_ISGID)
	{
		if (mode & S_IXGRP)
			buf[6] = 's';
		else
			buf[6] = 'S';
	}
	if (mode & S_ISVTX)
	{
		if (mode & S_IXOTH)
			buf[9] = 't';
		else
			buf[9] = 'T';
	}
}

void	fill_mode(mode_t mode, char *buf)
{
	fill_filetype(mode, buf);
	fill_rwx(mode, buf);
	fill_special(mode, buf);
	buf[10] = '\0';
}
