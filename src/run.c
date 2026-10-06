/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void	partition(t_file *all, t_file **errors, t_file **files,
				t_file **dirs)
{
	t_file	*next;
	t_file	*err_tail;
	t_file	*file_tail;
	t_file	*dir_tail;

	*errors = NULL;
	*files = NULL;
	*dirs = NULL;
	err_tail = NULL;
	file_tail = NULL;
	dir_tail = NULL;
	while (all)
	{
		next = all->next;
		all->next = NULL;
		if (!all->ok)
			append_file(errors, &err_tail, all);
		else if (all->is_dir)
			append_file(dirs, &dir_tail, all);
		else
			append_file(files, &file_tail, all);
		all = next;
	}
}

static int	load_operands(t_args *args, t_file **all, char *prog)
{
	t_file	*tail;
	t_file	*node;
	int		i;

	*all = NULL;
	tail = NULL;
	i = 0;
	while (i < args->path_count)
	{
		node = make_operand(args->paths[i], &args->flags);
		if (append_file(all, &tail, node) != 0)
		{
			free_files(*all);
			ft_putstr_fd(prog, 2);
			ft_putstr_fd(": malloc error\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	emit_errors(t_file *errors, char *prog, int *status)
{
	t_file	*it;

	it = errors;
	while (it)
	{
		print_error(prog, "cannot access", it->name, it->err_no);
		raise_status(status, 2);
		it = it->next;
	}
}

static void	list_all_dirs(t_file *dirs, t_args *args, char *prog,
				int *status)
{
	t_listdir	ctx;
	t_file		*it;
	int			printed;
	int			header;

	printed = 0;
	header = (args->path_count > 1) || args->flags.rec;
	it = dirs;
	while (it)
	{
		ctx.path = it->path;
		ctx.flags = &args->flags;
		ctx.prog = prog;
		ctx.show_header = header;
		ctx.need_blank = printed;
		ctx.open_fail = 2;
		ctx.status = status;
		if (list_directory(&ctx))
			printed = 1;
		it = it->next;
	}
}

int	run_ls(t_args *args, char *prog)
{
	t_file	*all;
	t_file	*errors;
	t_file	*files;
	t_file	*dirs;
	int		status;

	if (load_operands(args, &all, prog) != 0)
		return (2);
	partition(all, &errors, &files, &dirs);
	files = sort_files(files, &args->flags);
	dirs = sort_files(dirs, &args->flags);
	status = 0;
	emit_errors(errors, prog, &status);
	if (files)
		print_files(files, &args->flags, 0, dirs);
	if (files && dirs)
		ft_putchar_fd('\n', 1);
	list_all_dirs(dirs, args, prog, &status);
	free_files(errors);
	free_files(files);
	free_files(dirs);
	return (status);
}
