/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int	same_word(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return (a[i] == '\0' && b[i] == '\0');
}

int	is_flag_word(char *arg)
{
	if (arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	return (1);
}

int	set_flag(t_flags *flags, char option)
{
	if (option == 'l')
		flags->l = 1;
	else if (option == 'R')
		flags->rec = 1;
	else if (option == 'a')
		flags->a = 1;
	else if (option == 'r')
		flags->r = 1;
	else if (option == 't')
		flags->t = 1;
	else
		return (1);
	return (0);
}

int	read_flags(t_flags *flags, char *prog, char *arg)
{
	int	i;

	if (arg[1] == '-')
	{
		unrecognized_option(prog, arg);
		return (1);
	}
	i = 1;
	while (arg[i])
	{
		if (set_flag(flags, arg[i]) != 0)
		{
			illegal_option(prog, arg[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
