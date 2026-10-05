#include "../lib/ft_ls.h"

/*
 * Small helpers used across the project.
 *
 * print_error: prog: message 'path': strerror — single quotes, or double
 * quotes when the path contains an apostrophe (GNU ls).
 */

/* Byte-wise string compare (same order as ls with LC_ALL=C). */
int	ft_strcmp(char *a, char *b)
{
	return (ft_strncmp(a, b, ft_strlen(a) + ft_strlen(b) + 1));
}

/*
 * Builds "dir/name" without doubling the slash when dir already ends with /.
 * Returns a new string, or NULL on malloc failure.
 */
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

/*
 * Writes path quoted like GNU ls: 'name', or "name" if it contains a quote.
 */
static void	put_quoted(char *str)
{
	char	quote;

	quote = '\'';
	if (str && ft_strchr(str, '\''))
		quote = '"';
	ft_putchar_fd(quote, 2);
	ft_putstr_fd(str, 2);
	ft_putchar_fd(quote, 2);
}

/*
 * Prints "prog: phrase 'path': strerror(err)" on stderr.
 * err is the errno value to show (often saved before another call).
 */
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
