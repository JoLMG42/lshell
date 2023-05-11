/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:47:22 by lcalvie           #+#    #+#             */
/*   Updated: 2023/05/04 18:51:04 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checkstar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

void	cut_setwildcardsfirstcmd(int f, t_tree *tmp, char **tab)
{
	if (f)
	{
		free_tab(tmp->cmd_left->arg);
		tmp->cmd_left->arg = tab;
	}
	else
		free(tab);
}

void	setwildcardsfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		i;
	int		f;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	if (tmp->cmd_left->cmd)
		check_wildcards_first_cmd(lst);
	tab = NULL;
	i = 0;
	f = 0;
	while (tmp->cmd_left->arg[i])
	{
		if (checkstar(tmp->cmd_left->arg[i]))
		{
			free_tab(tab);
			tab = recreatearg(tmp->cmd_left->arg, tmp->cmd_left->arg[i]);
			f = 1;
		}
		i++;
	}
	cut_setwildcardsfirstcmd(f, tmp, tab);
}

char	**dup_tab(char **tab)
{
	int		i;
	char	**res;

	i = 0;
	res = malloc(sizeof(char *) * (tab_len(tab) + 1));
	if (!res)
		return (NULL);
	while (tab[i])
	{
		res[i] = ft_strdup(tab[i]);
		i++;
	}
	res[i] = 0;
	return (res);
}

void	setwildcards(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		i;

	tmp = (*lst)->next;
	while (tmp)
	{
		if (tmp->cmd_right->arg)
		{
			i = 0;
			while (tmp->cmd_right->arg[i])
			{
				if (checkstar(tmp->cmd_right->arg[i]))
				{
					tab = recreatearg(tmp->cmd_right->arg,
							tmp->cmd_right->arg[i]);
					free_tab(tmp->cmd_right->arg);
					tmp->cmd_right->arg = dup_tab(tab);
					free_tab(tab);
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
}
