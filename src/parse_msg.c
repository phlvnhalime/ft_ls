/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	illegal_option(char *prog, char option)
{
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": invalid option -- \'", 2);
	ft_putchar_fd(option, 2);
	ft_putstr_fd("\'\nTry \'", 2);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(" --help\' for more information.\n", 2);
}

void	unrecognized_option(char *prog, char *arg)
{
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": unrecognized option \'", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\'\nTry \'", 2);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(" --help\' for more information.\n", 2);
}
