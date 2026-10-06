/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int	is_dot(char *name)
{
	if (name[0] != '.')
		return (0);
	if (name[1] == '\0')
		return (1);
	if (name[1] == '.' && name[2] == '\0')
		return (1);
	return (0);
}

static void	print_dir_header(char *path, int show_header, int need_blank)
{
	if (need_blank)
		ft_putchar_fd('\n', 1);
	if (show_header)
	{
		ft_putstr_fd(path, 1);
		ft_putstr_fd(":\n", 1);
	}
}

static void	recurse_dirs(t_file *list, t_listdir *parent)
{
	t_listdir	child;
	t_file		*it;

	it = list;
	while (it)
	{
		if (it->is_dir && !is_dot(it->name))
		{
			child.path = it->path;
			child.flags = parent->flags;
			child.prog = parent->prog;
			child.show_header = 1;
			child.need_blank = 1;
			child.open_fail = 1;
			child.status = parent->status;
			list_directory(&child);
		}
		it = it->next;
	}
}

int	list_directory(t_listdir *ctx)
{
	t_file	*list;
	t_read	read_ctx;

	read_ctx.path = ctx->path;
	read_ctx.flags = ctx->flags;
	read_ctx.prog = ctx->prog;
	read_ctx.out = &list;
	read_ctx.status = ctx->status;
	read_ctx.open_fail = ctx->open_fail;
	if (read_directory(&read_ctx) != 0)
		return (0);
	print_dir_header(ctx->path, ctx->show_header, ctx->need_blank);
	list = sort_files(list, ctx->flags);
	print_files(list, ctx->flags, 1, NULL);
	if (ctx->flags->rec)
		recurse_dirs(list, ctx);
	free_files(list);
	return (1);
}
