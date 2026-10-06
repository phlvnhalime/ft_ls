/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_operand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	mark_operand(t_file *file, t_flags *flags)
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
		if (stat(file->path, &followed) == 0
			&& S_ISDIR(followed.st_mode))
			file->is_dir = 1;
	}
}

t_file	*make_operand(char *path, t_flags *flags)
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

void	raise_status(int *status, int code)
{
	if (code > *status)
		*status = code;
}
