#include "../lib/ft_ls.h"

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
	if (S_ISDIR(file->st.st_mode))
		file->is_dir = 1;
	else if (!flags->l && S_ISLNK(file->st.st_mode))
	{
		if (stat(file->path, &followed) == 0 && S_ISDIR(followed.st_mode))
			file->is_dir = 1;
	}
}

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

static int	read_directory(char *path, t_flags *flags, char *prog,
				t_file **out, int *status)
{
	DIR				*dir;
	struct dirent	*ent;
	t_file			*head;
	t_file			*tail;
	t_file			*node;
	char			*full;

	*out = NULL;
	dir = opendir(path);
	if (!dir)
	{
		print_error(prog, path, errno);
		*status = 1;
		return (1);
	}
	head = NULL;
	tail = NULL;
	errno = 0;
	while ((ent = readdir(dir)) != NULL)
	{
		if (!flags->a && ent->d_name[0] == '.')
			continue ;
		full = join_path(path, ent->d_name);
		node = new_file(ent->d_name, full);
		if (!node)
		{
			free_files(head);
			closedir(dir);
			print_error(prog, "malloc", errno);
			*status = 1;
			return (1);
		}
		if (lstat(node->path, &node->st) != 0)
		{
			print_error(prog, node->path, errno);
			free(node->name);
			free(node->path);
			free(node);
			*status = 1;
			errno = 0;
			continue ;
		}
		node->ok = 1;
		node->is_dir = S_ISDIR(node->st.st_mode);
		append_file(&head, &tail, node);
		errno = 0;
	}
	if (errno != 0)
	{
		print_error(prog, path, errno);
		free_files(head);
		closedir(dir);
		*status = 1;
		return (1);
	}
	closedir(dir);
	*out = head;
	return (0);
}

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

static void	list_directory(char *path, t_flags *flags, char *prog,
				int show_header, int *status)
{
	t_file	*list;
	t_file	*it;

	if (show_header)
	{
		ft_putstr_fd(1, path);
		ft_putstr_fd(1, ":\n");
	}
	if (read_directory(path, flags, prog, &list, status) != 0)
	{
		if (flags->l)
			ft_putstr_fd(1, "total 0\n");
		return ;
	}
	list = sort_files(list, flags);
	print_files(list, flags, 1);
	if (flags->R)
	{
		it = list;
		while (it)
		{
			if (it->is_dir && !is_dot(it->name))
			{
				ft_putchar_fd(1, '\n');
				list_directory(it->path, flags, prog, 1, status);
			}
			it = it->next;
		}
	}
	free_files(list);
}

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

static int	list_len(t_file *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

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
			print_error(prog, "malloc", errno);
			return (1);
		}
		i++;
	}
	partition(all, &errors, &files, &dirs);
	errors = sort_files(errors, &args->flags);
	files = sort_files(files, &args->flags);
	dirs = sort_files(dirs, &args->flags);
	it = errors;
	while (it)
	{
		print_error(prog, it->name, it->err_no);
		status = 1;
		it = it->next;
	}
	printed = 0;
	if (files)
	{
		print_files(files, &args->flags, 0);
		printed = 1;
	}
	header = (list_len(dirs) > 1) || (files != NULL);
	it = dirs;
	while (it)
	{
		if (printed)
			ft_putchar_fd(1, '\n');
		list_directory(it->path, &args->flags, prog, header, &status);
		printed = 1;
		it = it->next;
	}
	free_files(errors);
	free_files(files);
	free_files(dirs);
	return (status);
}
