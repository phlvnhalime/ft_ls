/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:27 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void	init_args(t_args *args)
{
	args->flags.l = 0;
	args->flags.rec = 0;
	args->flags.a = 0;
	args->flags.r = 0;
	args->flags.t = 0;
	args->paths = NULL;
	args->path_count = 0;
}

static int	alloc_paths(t_args *args, int ac, char *prog)
{
	args->paths = malloc(sizeof(char *) * (size_t)ac);
	if (!args->paths)
	{
		ft_putstr_fd(prog, 2);
		ft_putstr_fd(": malloc error\n", 2);
		return (2);
	}
	return (0);
}

static int	take_argument(t_args *args, char **av, int i, int *paths_only)
{
	if (!*paths_only && same_word(av[i], "--"))
	{
		*paths_only = 1;
		return (0);
	}
	if (!*paths_only && is_flag_word(av[i]))
	{
		if (read_flags(&args->flags, av[0], av[i]) != 0)
			return (1);
		return (0);
	}
	args->paths[args->path_count] = av[i];
	args->path_count++;
	return (0);
}

int	parse_args(int ac, char **av, t_args *args)
{
	int	i;
	int	paths_only;

	init_args(args);
	if (alloc_paths(args, ac, av[0]) != 0)
		return (2);
	i = 1;
	paths_only = 0;
	while (i < ac)
	{
		if (take_argument(args, av, i, &paths_only) != 0)
		{
			free(args->paths);
			args->paths = NULL;
			args->path_count = 0;
			return (2);
		}
		i++;
	}
	if (args->path_count == 0)
	{
		args->paths[0] = ".";
		args->path_count = 1;
	}
	return (0);
}
