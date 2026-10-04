#include "lib/ft_ls.h"

int	main(int ac, char **av)
{
	t_args	args;
	int		status;

	// 1. Parse the command line.
	//    Separate flags (-l, -R, -a, -r, -t) from paths.
	//    Combined flags like -la count as both -l and -a.
	//    If there is no path, use ".".
	//    A bad flag prints an error like ls and stops.
	status = parse_args(ac, av, &args);
	if (status == 0)
		status = run_ls(&args, av[0]);
	free(args.paths);
	return (status);

	// 2. Define one file structure.
	//    It holds the name, the full path, and the lstat result.
	//    Use it for paths from the command line and for names read inside a directory.

	// 3. List one directory with no flags.
	//    Open it, read the names, skip names that start with '.'.
	//    Sort them A to Z, and print one name per line.

	// 4. Handle paths given on the command line.
	//    A missing path prints an error and the program continues.
	//    Files are printed first. Directories come after.
	//    If there is more than one directory, print a header before each one.

	// 5. Add -a.
	//    Also show hidden names, including "." and "..".

	// 6. Add -r and -t.
	//    -t sorts by modification time, newest first, and uses the name when times are equal.
	//    -r reverses that order.

	// 7. Add -l.
	//    For each name, print type and permissions, link count, owner, group, size, date, and name.
	//    Print the total line first. Line up the columns.

	// 8. Add -R.
	//    After printing a directory, list each real subdirectory the same way.
	//    Skip "." and "..". Do not follow a symbolic link into another directory.

	// 9. Clean up.
	//    Free every list.
	//    Compare ls and ./ft_ls on the same arguments:
	//    a missing file, a mix of files and directories, ls -l, and ls -lR.

	return (status);
}