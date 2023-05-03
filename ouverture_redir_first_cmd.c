/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ouverture_redir_first_cmd.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:03:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 11:10:39 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_ouverturefirstcmd_3(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], "<<") == 0)
	{
		if (tab[i + 1] && tab[i + 2])
		{
			if (tmp->cmd_left->cmd)
				free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = ft_strdup(tab[i + 2]);
		}
		else
		{
			if (tmp->cmd_left->cmd)
				free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = NULL;
		}
		tmp->cmd_left->limiter = ft_strdup(tab[i + 1]);
		tmp->cmd_left->is_hd = 1;
		tmp->cmd_left->name_in = ft_strdup(tab[i + 1]);
		if (tmp->cmd_left->cmd)
			tmp->cmd_left->cmd = recalculcmd(tmp->cmd_left->cmd,
					tmp->cmd_left->name_in, "<<");
	}
	return (tmp);
}

t_tree	*cut_ouverturefirstcmd_2(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], ">>") == 0)
	{
		if (i > 0)
		{
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
		}
		if (i == 0)
		{
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = NULL;
		}
		tmp->cmd_left->name_out = ft_strdup(tab[i + 1]);
		tmp->cmd_left->mode_open = 2;
	}
	return (tmp);
}

t_tree	*cut_ouverturefirstcmd(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], ">") == 0)
	{
		free(tmp->cmd_left->cmd);
		if (i > 0)
			tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
		else
			tmp->cmd_left->cmd = NULL;
		tmp->cmd_left->name_out = ft_strdup(tab[i + 1]);
		tmp->cmd_left->mode_open = 1;
	}
	if (ft_strcmp(tab[i], "<") == 0)
	{
		tmp->cmd_left->name_in = ft_strdup(tab[i + 1]);
		tmp->cmd_left->mode_open = 1;
		tmp->cmd_left->cmd = recalculcmd(tmp->cmd_left->cmd,
				tmp->cmd_left->name_in, "<");
	}
	return (tmp);
}

void	ouverturefirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		i;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left || !tmp->cmd_left->cmd)
		return ;
	i = 0;
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	while (tab[i])
	{
		tmp = cut_ouverturefirstcmd(tmp, tab, i);
		tmp = cut_ouverturefirstcmd_2(tmp, tab, i);
		tmp = cut_ouverturefirstcmd_3(tmp, tab, i);
		i++;
	}
	free_tab(tab);
}
