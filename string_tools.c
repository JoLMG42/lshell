/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:37:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 16:12:45 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(char c)
{
	if ((c >= 'a' && c <= 'z') || c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

int	tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	len_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			break;
		i++;
	}
	return (i);
}

char	*resjoin(char **tab)
{
	int		i;
	char	*res;

	i = 1;
	res = ft_strdup(tab[0]);;
	while (tab[i])
	{
		res = ft_strjoin(res, tab[i]);
		i++;
	}
	return res;
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}


