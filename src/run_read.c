/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static int	dir_malloc_fail(char *prog, DIR *dir, t_file *head, int *status)
{
	free_files(head);
	closedir(dir);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": malloc error\n", 2);
	raise_status(status, 2);
	return (1);
}

static int	add_dir_entry(t_read *ctx, t_file **head, t_file **tail,
				struct dirent *ent)
{
	t_file	*node;

	node = new_file(ent->d_name, join_path(ctx->path, ent->d_name));
	if (!node)
		return (-1);
	fill_entry(node, ent->d_type);
	if (!node->ok && (ctx->flags->l || ctx->flags->t))
	{
		print_error(ctx->prog, "cannot access", node->path, node->err_no);
		raise_status(ctx->status, 1);
	}
	append_file(head, tail, node);
	return (0);
}

static int	open_dir_fail(t_read *ctx)
{
	print_error(ctx->prog, "cannot open directory", ctx->path, errno);
	raise_status(ctx->status, ctx->open_fail);
	return (1);
}

static int	read_loop(t_read *ctx, DIR *dir, t_file **head, t_file **tail)
{
	struct dirent	*ent;

	errno = 0;
	ent = readdir(dir);
	while (ent)
	{
		if (ctx->flags->a || ent->d_name[0] != '.')
		{
			if (add_dir_entry(ctx, head, tail, ent) < 0)
				return (-1);
		}
		errno = 0;
		ent = readdir(dir);
	}
	return (0);
}

int	read_directory(t_read *ctx)
{
	DIR		*dir;
	t_file	*head;
	t_file	*tail;

	*ctx->out = NULL;
	dir = opendir(ctx->path);
	if (!dir)
		return (open_dir_fail(ctx));
	head = NULL;
	tail = NULL;
	if (read_loop(ctx, dir, &head, &tail) < 0)
		return (dir_malloc_fail(ctx->prog, dir, head, ctx->status));
	if (errno != 0)
	{
		print_error(ctx->prog, "reading directory", ctx->path, errno);
		free_files(head);
		closedir(dir);
		raise_status(ctx->status, 1);
		return (1);
	}
	closedir(dir);
	*ctx->out = head;
	return (0);
}
