/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:05:10 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 16:16:56 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_charset(const char str, char c)
{
	if (str == c)
		return (1);
	return (0);
}

static int	ft_count_word(const char *str, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (ft_check_charset(str[i], c) == 0)
			i++;
		else
		{
			while (str[i] && ft_check_charset(str[i], c) == 1)
				i++;
			count++;
		}
	}
	return (count);
}

static int	ft_lenword(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && ft_check_charset(str[i], c) == 0)
	{
		i++;
	}
	return (i);
}

static char	*ft_cpy(const char *str, char c, int n)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_lenword(str, c) + 1));
	if (!cpy)
		return (0);
	while (str[i] && i < n)
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	**ft_supersplit_lucie(char *s, char c)
{
	int		i;
	int		j;
	char	**str;

	i = 0;
	j = 0;
	str = malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!str)
		return (0);
	while (s[i])
	{
		if (ft_check_charset(s[i], c) == 1)
			i++;
		else
		{
			str[j] = ft_cpy(s + i, c, ft_lenword(s + i, c));
			j++;
			i = i + ft_lenword(s + i, c);
		}
	}
	str[j] = 0;
	return (str);
}
