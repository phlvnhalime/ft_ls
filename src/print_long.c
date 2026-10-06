/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	print_link_target(t_file *file)
{
	char	target[4096];
	ssize_t	n;

	if (!file->ok || !S_ISLNK(file->st.st_mode))
		return ;
	n = readlink(file->path, target, sizeof(target) - 1);
	if (n < 0)
		return ;
	target[n] = '\0';
	ft_putstr_fd(" -> ", 1);
	ft_putstr_fd(target, 1);
}

void	put_left(char *s, int width)
{
	ft_putstr_fd(s, 1);
	pad_spaces(width - (int)ft_strlen(s));
}

void	put_right(char *s, int width)
{
	pad_spaces(width - (int)ft_strlen(s));
	ft_putstr_fd(s, 1);
}

void	fill_lfields(t_file *file, t_lfields *f)
{
	if (!file->ok)
	{
		f->mode[0] = file->type;
		ft_memset(f->mode + 1, '?', 9);
		f->mode[10] = '\0';
		ft_strlcpy(f->nlink, "?", sizeof(f->nlink));
		ft_strlcpy(f->user, "?", sizeof(f->user));
		ft_strlcpy(f->group, "?", sizeof(f->group));
		ft_strlcpy(f->size, "?", sizeof(f->size));
		ft_strlcpy(f->date, "?", sizeof(f->date));
		return ;
	}
	fill_mode(file->st.st_mode, f->mode);
	ull_to_str((unsigned long long)file->st.st_nlink, f->nlink);
	owner_name(file->st.st_uid, f->user, sizeof(f->user));
	group_name(file->st.st_gid, f->group, sizeof(f->group));
	fill_size(&file->st, f->size);
	format_date(file->st.st_mtime, f->date);
}

void	print_long_line(t_file *file, t_width *width)
{
	t_lfields	f;

	fill_lfields(file, &f);
	ft_putstr_fd(f.mode, 1);
	ft_putchar_fd(' ', 1);
	put_right(f.nlink, width->nlink);
	ft_putchar_fd(' ', 1);
	put_left(f.user, width->user);
	ft_putchar_fd(' ', 1);
	put_left(f.group, width->group);
	ft_putchar_fd(' ', 1);
	put_right(f.size, width->size);
	ft_putchar_fd(' ', 1);
	put_right(f.date, 12);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd(file->name, 1);
	print_link_target(file);
	ft_putchar_fd('\n', 1);
}
