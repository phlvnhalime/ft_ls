#include "../lib/ft_ls.h"

/*
 * Command-line parsing.
 *
 * Supports combined flags (-laR), flags after operands, and "--".
 * Rejects unknown letters and unsupported long options (--foo).
 * Default operand is "." when none is given.
 */

/* Prints GNU ls style message for an unknown short option letter. */
static void	illegal_option(char *prog, char option)
{
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": invalid option -- '", 2);
	ft_putchar_fd(option, 2);
	ft_putstr_fd("'\nTry '", 2);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(" --help' for more information.\n", 2);
}

/* Prints GNU ls style message for an unsupported long option (--foo). */
static void	unrecognized_option(char *prog, char *arg)
{
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": unrecognized option '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("'\nTry '", 2);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(" --help' for more information.\n", 2);
}

/* Returns 1 if a and b are the same C string, otherwise 0. */
static int	same_word(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return (a[i] == '\0' && b[i] == '\0');
}

/*
 * Returns 1 if arg looks like an option word (-l, -la, --help).
 * A lone "-" is a path, not an option.
 */
static int	is_flag_word(char *arg)
{
	if (arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	return (1);
}

/*
 * Turns on one flag letter.
 * Supported: l (long), R (recursive), a (all), r (reverse), t (time).
 * Returns 0 on success, 1 if the letter is unknown.
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

/*
 * Reads one option word such as "-laR".
 * Long options ("--all") are rejected like GNU ls.
 * Returns 0 on success, 1 after printing an error.
 */
static int	read_flags(t_flags *flags, char *prog, char *arg)
{
	int	i;

	if (arg[1] == '-')
	{
		unrecognized_option(prog, arg);
		return (1);
	}
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

/* Clears flags and path list before parsing argv. */
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

/*
 * Allocates the operand array (at most ac pointers).
 * Returns 0 on success, 2 on malloc failure.
 */
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

/*
 * Sorts one argv word: "--" ends options, -flags set options, else path.
 * Returns 0 on success, 1 on an invalid option.
 */
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

/*
 * Fills args from the command line. Uses "." when no path is given.
 * Returns 0 on success, 2 on bad option or malloc failure.
 */
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
