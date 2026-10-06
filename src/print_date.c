/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_date.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:52:19 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static int	is_recent(time_t mtime)
{
	time_t	now;

	now = time(NULL);
	if (mtime > now)
		return (0);
	return ((now - mtime) <= (time_t)15778476);
}

static void	copy_date_base(char *ct, char *out)
{
	out[0] = ct[4];
	out[1] = ct[5];
	out[2] = ct[6];
	out[3] = ' ';
	out[4] = ct[8];
	out[5] = ct[9];
	out[6] = ' ';
}

static void	copy_date_time(char *ct, char *out)
{
	out[7] = ct[11];
	out[8] = ct[12];
	out[9] = ct[13];
	out[10] = ct[14];
	out[11] = ct[15];
}

static void	copy_date_year(char *ct, char *out)
{
	out[7] = ' ';
	out[8] = ct[20];
	out[9] = ct[21];
	out[10] = ct[22];
	out[11] = ct[23];
}

void	format_date(time_t mtime, char *out)
{
	char	*ct;

	ct = ctime(&mtime);
	if (!ct)
	{
		out[0] = '\0';
		return ;
	}
	copy_date_base(ct, out);
	if (is_recent(mtime))
		copy_date_time(ct, out);
	else
		copy_date_year(ct, out);
	out[12] = '\0';
}
