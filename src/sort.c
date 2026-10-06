/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static int	cmp_time(t_file *a, t_file *b)
{
	long long	d;

	d = (long long)a->st.st_mtime - (long long)b->st.st_mtime;
	if (d != 0)
		return ((d > 0) - (d < 0));
	d = (long long)a->st.st_mtim.tv_nsec
		- (long long)b->st.st_mtim.tv_nsec;
	if (d != 0)
		return ((d > 0) - (d < 0));
	return (0);
}

static int	cmp_files(t_file *a, t_file *b, t_flags *flags)
{
	int	cmp;

	cmp = 0;
	if (flags->t && a->ok && b->ok)
		cmp = -cmp_time(a, b);
	if (cmp == 0)
		cmp = ft_strcmp(a->name, b->name);
	if (flags->r)
		cmp = -cmp;
	return (cmp);
}

static t_file	*merge_files(t_file *a, t_file *b, t_flags *flags)
{
	t_file	dummy;
	t_file	*tail;

	tail = &dummy;
	dummy.next = NULL;
	while (a && b)
	{
		if (cmp_files(a, b, flags) <= 0)
		{
			tail->next = a;
			a = a->next;
		}
		else
		{
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}
	if (a)
		tail->next = a;
	else
		tail->next = b;
	return (dummy.next);
}

static void	split_files(t_file *src, t_file **left, t_file **right)
{
	t_file	*slow;
	t_file	*fast;

	slow = src;
	fast = src->next;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	*left = src;
	*right = slow->next;
	slow->next = NULL;
}

t_file	*sort_files(t_file *list, t_flags *flags)
{
	t_file	*left;
	t_file	*right;

	if (!list || !list->next)
		return (list);
	split_files(list, &left, &right);
	left = sort_files(left, flags);
	right = sort_files(right, flags);
	return (merge_files(left, right, flags));
}
