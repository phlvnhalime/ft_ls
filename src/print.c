/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:33 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	print_total(t_file *list)
{
	long long	total;

	total = 0;
	while (list)
	{
		if (list->ok)
			total += block_units(&list->st);
		list = list->next;
	}
	ft_putstr_fd("total ", 1);
	put_ull((unsigned long long)total);
	ft_putchar_fd('\n', 1);
}

static void	print_plain(t_file *list)
{
	while (list)
	{
		ft_putstr_fd(list->name, 1);
		ft_putchar_fd('\n', 1);
		list = list->next;
	}
}

static void	print_long(t_file *list, t_file *extra, int as_dir)
{
	t_width	width;
	t_file	*it;

	if (as_dir)
		print_total(list);
	measure(list, &width);
	measure_add(extra, &width);
	it = list;
	while (it)
	{
		print_long_line(it, &width);
		it = it->next;
	}
}

void	print_files(t_file *list, t_flags *flags, int as_dir, t_file *extra)
{
	if (flags->l)
		print_long(list, extra, as_dir);
	else
		print_plain(list);
}
