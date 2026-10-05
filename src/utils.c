#include "../lib/ft_ls.h"

int	ft_strcmp(char *a, char *b)
{
	return (ft_strncmp(a, b, ft_strlen(a) + ft_strlen(b) + 1));
}

char	*join_path(char *dir, char *name)
{
	size_t	len_dir;
	size_t	len_name;
	size_t	i;
	size_t	j;
	int		sep;
	char	*path;

	len_dir = ft_strlen(dir);
	len_name = ft_strlen(name);
	sep = 1;
	if (len_dir > 0 && dir[len_dir - 1] == '/')
		sep = 0;
	path = malloc(len_dir + (size_t)sep + len_name + 1);
	if (!path)
		return (NULL);
	i = 0;
	while (i < len_dir)
	{
		path[i] = dir[i];
		i++;
	}
	if (sep)
		path[i++] = '/';
	j = 0;
	while (j < len_name)
		path[i++] = name[j++];
	path[i] = '\0';
	return (path);
}

static void	put_quoted(char *str)
{
	ft_putchar_fd('\'', 2);
	while (str && *str)
	{
		if (*str == '\'')
			ft_putstr_fd("'\\''", 2);
		else
			ft_putchar_fd(*str, 2);
		str++;
	}
	ft_putchar_fd('\'', 2);
}

void	print_error(char *prog, char *phrase, char *path, int err)
{
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(phrase, 2);
	ft_putchar_fd(' ', 2);
	put_quoted(path);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(err), 2);
	ft_putchar_fd('\n', 2);
}