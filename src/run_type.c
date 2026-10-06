/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

char	type_from_dirent(unsigned char d_type)
{
	if (d_type == DT_REG)
		return ('-');
	if (d_type == DT_DIR)
		return ('d');
	if (d_type == DT_LNK)
		return ('l');
	if (d_type == DT_CHR)
		return ('c');
	if (d_type == DT_BLK)
		return ('b');
	if (d_type == DT_FIFO)
		return ('p');
	if (d_type == DT_SOCK)
		return ('s');
	return ('?');
}

char	type_from_mode(mode_t mode)
{
	if (S_ISREG(mode))
		return ('-');
	if (S_ISDIR(mode))
		return ('d');
	if (S_ISLNK(mode))
		return ('l');
	if (S_ISCHR(mode))
		return ('c');
	if (S_ISBLK(mode))
		return ('b');
	if (S_ISFIFO(mode))
		return ('p');
	if (S_ISSOCK(mode))
		return ('s');
	return ('?');
}

void	fill_entry(t_file *node, unsigned char d_type)
{
	if (lstat(node->path, &node->st) == 0)
	{
		node->ok = 1;
		node->type = type_from_mode(node->st.st_mode);
		node->is_dir = S_ISDIR(node->st.st_mode);
		return ;
	}
	node->ok = 0;
	node->err_no = errno;
	node->type = type_from_dirent(d_type);
	node->is_dir = (d_type == DT_DIR);
	ft_bzero(&node->st, sizeof(node->st));
}
