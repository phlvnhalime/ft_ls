/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	free_files(t_file *list)
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

t_file	*new_file(char *name, char *path)
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

int	append_file(t_file **head, t_file **tail, t_file *node)
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
