/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:41:19 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/02 17:54:13 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}

int	ft_lstsize(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	*cut_rejointab(char **tab, int h, char **res, int j)
{
	if (j == 0 && h == 0)
	{
		res[j] = NULL;
		res[j] = ft_strdup(tab[h]);
		if (tab[h + 1])
			res[j] = ft_strjoin(res[j], " ");
	}
	else
	{
		res[j] = ft_strjoin(res[j], tab[h]);
		res[j] = ft_strjoin(res[j], " ");
	}
	return (res[j]);
}

char	**rejointab(char **tab)
{
	int		i;
	int		j;
	int		h;
	char	**res;

	i = 0;
	j = 0;
	h = 0;
	res = malloc(sizeof(char *) * (10000));
	while (tab[h])
	{
		if (tab[h + 1] && ft_strcmp(tab[h], "|") == 0
			|| ft_strcmp(tab[h], "&&") == 0 || ft_strcmp(tab[h], "||") == 0)
		{
			j++;
			res[j] = NULL;
			res[j] = ft_strjoin(res[j], tab[h]);
			if (tab[h + 1] && ft_strcmp(tab[h + 1], "|") == 0
				|| ft_strcmp(tab[h + 1], "&&") == 0
				|| ft_strcmp(tab[h + 1], "||") == 0);
			else
			{
				j++;
				res[j] = NULL;
			}
		}
		else
			res[j] = cut_rejointab(tab, h, res, j);
		h++;
	}
	free_tab(tab);
	res[j + 1] = 0;
	return (res);
}
