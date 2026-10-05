/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:16:05 by hpehliva          #+#    #+#             */
/*   Updated: 2024/10/17 15:41:06 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (0);
}

static int	ft_count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			i++;
		else
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*ft_put_in(char *letter, char const *s, int i, int ltr_len)
{
	int	j;

	j = 0;
	while (ltr_len > 0)
	{
		letter[j] = s[i - ltr_len];
		j++;
		ltr_len--;
	}
	letter[j] = '\0';
	return (letter);
}

static char	**ft_split_words(char const *s, char c, char **s2, int nb)
{
	int	i;
	int	letter;
	int	ltr_len;

	i = 0;
	letter = 0;
	ltr_len = 0;
	while (letter < nb)
	{
		while (s[i] && s[i] == c)
			i++;
		while (s[i] && s[i] != c)
		{
			i++;
			ltr_len++;
		}
		s2[letter] = (char *)malloc(sizeof(char) * (ltr_len + 1));
		if (!s2[letter])
			return (free_array(s2, letter));
		ft_put_in(s2[letter], s, i, ltr_len);
		ltr_len = 0;
		letter++;
	}
	s2[letter] = 0;
	return (s2);
}

char	**ft_split(char const *s, char c)
{
	char			**s2;
	unsigned int	num_words;

	if (!s)
		return (0);
	num_words = ft_count_words(s, c);
	s2 = (char **)malloc(sizeof(char *) * (num_words + 1));
	if (!s2)
		return (0);
	s2 = ft_split_words(s, c, s2, num_words);
	return (s2);
}
