#include "../lib/ft_ls.h"

size_t	ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_strcmp(char *a, char *b)
{
	size_t	i;

	i = 0;
	while (a[i] && (unsigned char)a[i] == (unsigned char)b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

char	*ft_strdup(char *str)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = ft_strlen(str);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
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

void	ft_putchar_fd(int fd, char c)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(int fd, char *str)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

void	print_error(char *prog, char *path, int err)
{
	ft_putstr_fd(2, prog);
	ft_putstr_fd(2, ": ");
	ft_putstr_fd(2, path);
	ft_putstr_fd(2, ": ");
	ft_putstr_fd(2, strerror(err));
	ft_putstr_fd(2, "\n");
}