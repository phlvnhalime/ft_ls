/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int	ft_strcmp(char *a, char *b)
{
	return (ft_strncmp(a, b, ft_strlen(a) + ft_strlen(b) + 1));
}

char	*join_path(char *dir, char *name)
{
	char	*path;
	size_t	dlen;
	size_t	nlen;
	size_t	pos;
	int		need_sep;

	dlen = ft_strlen(dir);
	nlen = ft_strlen(name);
	need_sep = !(dlen > 0 && dir[dlen - 1] == '/');
	path = malloc(dlen + (size_t)need_sep + nlen + 1);
	if (!path)
		return (NULL);
	ft_memcpy(path, dir, dlen);
	pos = dlen;
	if (need_sep)
		path[pos++] = '/';
	ft_memcpy(path + pos, name, nlen + 1);
	return (path);
}

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
