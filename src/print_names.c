/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_names.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:32 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void	copy_str(char *dst, char *src, size_t cap)
{
	size_t	i;

	i = 0;
	if (!src)
		src = "";
	while (src[i] && i + 1 < cap)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void	owner_name(uid_t uid, char *buf, size_t cap)
{
	struct passwd	*pw;

	pw = getpwuid(uid);
	if (pw)
		copy_str(buf, pw->pw_name, cap);
	else
		ull_to_str((unsigned long long)uid, buf);
	(void)cap;
}

void	group_name(gid_t gid, char *buf, size_t cap)
{
	struct group	*gr;

	gr = getgrgid(gid);
	if (gr)
		copy_str(buf, gr->gr_name, cap);
	else
		ull_to_str((unsigned long long)gid, buf);
	(void)cap;
}
