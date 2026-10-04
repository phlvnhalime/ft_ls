#include "../lib/ft_ls.h"


static void	illegal_option(char *prog, char option)
{
	char	letter[2];

	letter[0] = option;
	letter[1] = '\0';
	ft_putstr_fd(2, prog);
	ft_putstr_fd(2, ": invalid option -- ");
	ft_putstr_fd(2, letter);
	ft_putstr_fd(2, "\nusage: ");
	ft_putstr_fd(2, prog);
	ft_putstr_fd(2, " [-Ralrt] [file ...]\n");
}

static int	same_word(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return (a[i] == '\0' && b[i] == '\0');
}

static int	is_flag_word(char *arg)
{
	if (arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	return (1);
}
/*
	This function sets the flag to true if the option is found in the argument.
		l -> long format
		R -> recursive
		a -> all files
		r -> reverse
		t -> time
	if the option is not found, it returns 1.
	if the option is found, it returns 0.
*/
static int	set_flag(t_flags *flags, char option)
{
	if (option == 'l')
		flags->l = true;
	else if (option == 'R')
		flags->R = true;
	else if (option == 'a')
		flags->a = true;
	else if (option == 'r')
		flags->r = true;
	else if (option == 't')
		flags->t = true;
	else
		return (1);
	return (0);
}

static int	read_flags(t_flags *flags, char *prog, char *arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (set_flag(flags, arg[i]) != 0)
		{
			illegal_option(prog, arg[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	init_args(t_args *args)
{
	args->flags.l = false;
	args->flags.R = false;
	args->flags.a = false;
	args->flags.r = false;
	args->flags.t = false;
	args->paths = NULL;
	args->path_count = 0;
}

static int	alloc_paths(t_args *args, int ac, char *prog)
{
	args->paths = malloc(sizeof(char *) * (size_t)ac);
	if (!args->paths)
	{
		ft_putstr_fd(2, prog);
		ft_putstr_fd(2, ": malloc error\n");
		return (1);
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
	*paths_only = 1;
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
		return (1);
	i = 1;
	paths_only = 0;
	while (i < ac)
	{
		if (take_argument(args, av, i, &paths_only) != 0)
		{
			free(args->paths);
			args->paths = NULL;
			args->path_count = 0;
			return (1);
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
