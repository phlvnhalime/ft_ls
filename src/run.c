#include "../lib/ft_ls.h"

/*
 * Directory traversal and operand handling.
 *
 * Operands: errors in argv order (not sorted), then files, then dirs.
 * A blank line separates file operands from directory listings.
 *
 * Inside a directory: skip hidden names unless -a. Each entry is lstat'd;
 * if that fails the name is kept (d_type for type/is_dir). With -l or -t,
 * "cannot access" is printed. Unsearchable dirs still list names like ls.
 *
 * -R recurses into subdirs (not "." / ".."; is_dir from lstat, no symlink follow).
 */

/* Frees every node in a list, including name and path strings. */
static void	free_files(t_file *list)
{
	t_file	*next;

	while (list)
	{
		next = list->next;
		free(list->name);
		free(list->path);
		free(list);
		list = next;
	}
}

/*
 * Maps readdir's d_type to the first character of the -l mode string.
 * Used when lstat failed but we still need to list the name.
 */
static char	type_from_dirent(unsigned char d_type)
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

/* Maps st_mode to the first character of the -l mode string. */
static char	type_from_mode(mode_t mode)
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

/*
 * Allocates one list entry. name is copied; path is taken over (must be
 * heap memory). Returns NULL on failure with nothing leaked.
 */
static t_file	*new_file(char *name, char *path)
{
	t_file	*file;

	file = malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	file->name = ft_strdup(name);
	file->path = path;
	file->ok = 0;
	file->is_dir = 0;
	file->err_no = 0;
	file->type = '?';
	file->next = NULL;
	if (!file->name || !file->path)
	{
		free(file->name);
		free(file->path);
		free(file);
		return (NULL);
	}
	return (file);
}

/* Appends node at the end of a list (head/tail). Returns 1 if node is NULL. */
static int	append_file(t_file **head, t_file **tail, t_file *node)
{
	if (!node)
		return (1);
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
	node->next = NULL;
	return (0);
}

/*
 * Stats a command-line operand with lstat.
 * Without -l, a symlink to a directory is treated as that directory.
 */
static void	mark_operand(t_file *file, t_flags *flags)
{
	struct stat	followed;

	if (lstat(file->path, &file->st) != 0)
	{
		file->ok = 0;
		file->err_no = errno;
		return ;
	}
	file->ok = 1;
	file->type = type_from_mode(file->st.st_mode);
	if (S_ISDIR(file->st.st_mode))
		file->is_dir = 1;
	else if (!flags->l && S_ISLNK(file->st.st_mode))
	{
		if (stat(file->path, &followed) == 0 && S_ISDIR(followed.st_mode))
			file->is_dir = 1;
	}
}

/* Builds one operand entry from a path given on the command line. */
static t_file	*make_operand(char *path, t_flags *flags)
{
	t_file	*file;
	char	*owned;

	owned = ft_strdup(path);
	if (!owned)
		return (NULL);
	file = new_file(path, owned);
	if (!file)
		return (NULL);
	mark_operand(file, flags);
	return (file);
}

/* Raises *status only if code is more serious (1 minor, 2 serious). */
static void	raise_status(int *status, int code)
{
	if (code > *status)
		*status = code;
}

/*
 * Fills a directory entry from lstat. On failure the entry is kept:
 * type and is_dir come from readdir's d_type so plain and -R listings
 * still work when the directory is not searchable.
 */
static void	fill_entry(t_file *node, unsigned char d_type)
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

/* Cleans up and reports a malloc failure while reading a directory. */
static int	dir_malloc_fail(char *prog, DIR *dir, t_file *head, int *status)
{
	free_files(head);
	closedir(dir);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": malloc error\n", 2);
	raise_status(status, 2);
	return (1);
}

/*
 * Opens path and builds a list of its entries (-a shows hidden names).
 * open_fail is the exit code when opendir fails (2 for operands, 1 for -R).
 * Returns 0 on success, 1 on failure (*out is NULL then).
 */
static int	read_directory(char *path, t_flags *flags, char *prog,
				t_file **out, int *status, int open_fail)
{
	DIR				*dir;
	struct dirent	*ent;
	t_file			*head;
	t_file			*tail;
	t_file			*node;

	*out = NULL;
	dir = opendir(path);
	if (!dir)
	{
		print_error(prog, "cannot open directory", path, errno);
		raise_status(status, open_fail);
		return (1);
	}
	head = NULL;
	tail = NULL;
	errno = 0;
	ent = readdir(dir);
	while (ent)
	{
		if (flags->a || ent->d_name[0] != '.')
		{
			node = new_file(ent->d_name, join_path(path, ent->d_name));
			if (!node)
				return (dir_malloc_fail(prog, dir, head, status));
			fill_entry(node, ent->d_type);
			if (!node->ok && (flags->l || flags->t))
			{
				print_error(prog, "cannot access", node->path, node->err_no);
				raise_status(status, 1);
			}
			append_file(&head, &tail, node);
		}
		errno = 0;
		ent = readdir(dir);
	}
	if (errno != 0)
	{
		print_error(prog, "reading directory", path, errno);
		free_files(head);
		closedir(dir);
		raise_status(status, 1);
		return (1);
	}
	closedir(dir);
	*out = head;
	return (0);
}

/* Returns 1 for "." and "..", which -R must never descend into. */
static int	is_dot(char *name)
{
	if (name[0] != '.')
		return (0);
	if (name[1] == '\0')
		return (1);
	if (name[1] == '.' && name[2] == '\0')
		return (1);
	return (0);
}

/*
 * Lists one directory: read, sort, print, then with -R each subdirectory.
 * Returns 1 if the directory was opened, 0 if opendir failed.
 */
static int	list_directory(char *path, t_flags *flags, char *prog,
				int show_header, int need_blank, int open_fail, int *status)
{
	t_file	*list;
	t_file	*it;

	if (read_directory(path, flags, prog, &list, status, open_fail) != 0)
		return (0);
	if (need_blank)
		ft_putchar_fd('\n', 1);
	if (show_header)
	{
		ft_putstr_fd(path, 1);
		ft_putstr_fd(":\n", 1);
	}
	list = sort_files(list, flags);
	print_files(list, flags, 1, NULL);
	if (flags->R)
	{
		it = list;
		while (it)
		{
			if (it->is_dir && !is_dot(it->name))
				list_directory(it->path, flags, prog, 1, 1, 1, status);
			it = it->next;
		}
	}
	free_files(list);
	return (1);
}

/*
 * Splits operands into three lists, keeping argv order inside each:
 * failed paths, plain files, directories.
 */
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

/*
 * Main listing: errors (argv order), then files, then directories.
 * Headers appear with several operands or with -R.
 * Returns the process exit status (0, 1 or 2).
 */
int	run_ls(t_args *args, char *prog)
{
	t_file	*all;
	t_file	*tail;
	t_file	*errors;
	t_file	*files;
	t_file	*dirs;
	t_file	*it;
	int		status;
	int		i;
	int		printed;
	int		header;

	all = NULL;
	tail = NULL;
	status = 0;
	i = 0;
	while (i < args->path_count)
	{
		if (append_file(&all, &tail,
				make_operand(args->paths[i], &args->flags)) != 0)
		{
			free_files(all);
			ft_putstr_fd(prog, 2);
			ft_putstr_fd(": malloc error\n", 2);
			return (2);
		}
		i++;
	}
	partition(all, &errors, &files, &dirs);
	files = sort_files(files, &args->flags);
	dirs = sort_files(dirs, &args->flags);
	it = errors;
	while (it)
	{
		print_error(prog, "cannot access", it->name, it->err_no);
		raise_status(&status, 2);
		it = it->next;
	}
	printed = 0;
	if (files)
		print_files(files, &args->flags, 0, dirs);
	if (files && dirs)
		ft_putchar_fd('\n', 1);
	header = (args->path_count > 1) || args->flags.R;
	it = dirs;
	while (it)
	{
		if (list_directory(it->path, &args->flags, prog, header, printed, 2,
				&status))
			printed = 1;
		it = it->next;
	}
	free_files(errors);
	free_files(files);
	free_files(dirs);
	return (status);
}
