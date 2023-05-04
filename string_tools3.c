/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:41:19 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 16:56:50 by jtaravel         ###   ########.fr       */
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

char	**cut_rejointab_2(char **tab, int h, t_s *s, char **res)
{
	s->j++;
	res[s->j] = NULL;
	res[s->j] = ft_strjoin(res[s->j], tab[h]);
	if (tab[h + 1] && ft_strcmp(tab[h + 1], "|") == 0
		|| ft_strcmp(tab[h + 1], "&&") == 0
		|| ft_strcmp(tab[h + 1], "||") == 0);
	else
	{
		s->j++;
		res[s->j] = NULL;
	}
	return (res);
}

char	**rejointab(char **tab)
{
	t_s		s;
	int		h;
	char	**res;

	init_syntax_struct(&s);
	h = -1;
	res = malloc(sizeof(char *) * (tab_len(tab) + 100));
	while (tab[++h])
	{
		if (tab[h + 1] && ft_strcmp(tab[h], "|") == 0
			|| ft_strcmp(tab[h], "&&") == 0 || ft_strcmp(tab[h], "||") == 0)
		{
			res = cut_rejointab_2(tab, h, &s, res);
		}
		else
			res[s.j] = cut_rejointab(tab, h, res, s.j);
	}
	free_tab(tab);
	res[s.j + 1] = 0;
	return (res);
}
