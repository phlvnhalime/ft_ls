/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_measure.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:30 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void	measure_bad(t_width *width)
{
	widen(&width->nlink, 1);
	widen(&width->user, 1);
	widen(&width->group, 1);
	widen(&width->size, 1);
}

static void	measure_one(t_file *file, t_width *width)
{
	char	user[256];
	char	group[256];
	char	size[64];

	if (!file->ok)
	{
		measure_bad(width);
		return ;
	}
	widen(&width->nlink, ull_len((unsigned long long)file->st.st_nlink));
	owner_name(file->st.st_uid, user, sizeof(user));
	group_name(file->st.st_gid, group, sizeof(group));
	fill_size(&file->st, size);
	widen(&width->user, (int)ft_strlen(user));
	widen(&width->group, (int)ft_strlen(group));
	widen(&width->size, (int)ft_strlen(size));
}

void	measure_add(t_file *list, t_width *width)
{
	while (list)
	{
		measure_one(list, width);
		list = list->next;
	}
}

void	measure(t_file *list, t_width *width)
{
	width->nlink = 1;
	width->user = 0;
	width->group = 0;
	width->size = 1;
	measure_add(list, width);
}
