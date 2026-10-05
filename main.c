#include "lib/ft_ls.h"

/*
 * ft_ls — mandatory part only (42 subject).
 *
 * Flow:
 *   1. parse_args  — flags (-l -R -a -r -t) and operands; default "."
 *   2. run_ls      — list everything and return exit status (0 / 1 / 2)
 *
 * Compare with system ls: bash tests/run_tests.sh -v
 *
 * Exit codes (GNU ls):
 *   0 — success
 *   1 — minor problem (e.g. unreadable entry inside a directory)
 *   2 — serious (bad option, missing operand, malloc failure)
 */
int	main(int ac, char **av)
{
	t_args	args;
	int		status;

	status = parse_args(ac, av, &args);
	if (status == 0)
		status = run_ls(&args, av[0]);
	free(args.paths);
	return (status);
}
