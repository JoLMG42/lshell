/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ouverture_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:10:53 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 11:18:11 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_ouvertureredir_3(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], "<<") == 0)
	{
		if (tab[i + 1] && tab[i + 2])
		{
			if (tmp->cmd_right->cmd)
				free(tmp->cmd_right->cmd);
			tmp->cmd_right->cmd = ft_strdup(tab[i + 2]);
		}
		else
		{
			if (tmp->cmd_right->cmd)
				free(tmp->cmd_right->cmd);
			tmp->cmd_right->cmd = NULL;
		}
		tmp->cmd_right->limiter = ft_strdup(tab[i + 1]);
		tmp->cmd_right->is_hd = 1;
		tmp->cmd_right->name_in = ft_strdup(tab[i + 1]);
		if (tmp->cmd_right->cmd)
			tmp->cmd_right->cmd = recalculcmd(tmp->cmd_right->cmd,
					tmp->cmd_right->name_in, "<<");
	}
	return (tmp);
}

t_tree	*cut_ouvertureredir_2(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], ">>") == 0)
	{
		if (i > 0)
		{
			free(tmp->cmd_right->cmd);
			tmp->cmd_right->cmd = ft_strdup(tab[i - 1]);
		}
		if (i == 0)
		{
			free(tmp->cmd_right->cmd);
			tmp->cmd_right->cmd = NULL;
		}
		tmp->cmd_right->name_out = ft_strdup(tab[i + 1]);
		tmp->cmd_right->mode_open = 2;
	}
	return (tmp);
}

t_tree	*cut_ouvertureredir(t_tree *tmp, char **tab, int i)
{
	if (ft_strcmp(tab[i], ">") == 0)
	{
		free(tmp->cmd_right->cmd);
		if (i > 0)
			tmp->cmd_right->cmd = ft_strdup(tab[i - 1]);
		else
			tmp->cmd_right->cmd = NULL;
		tmp->cmd_right->name_out = ft_strdup(tab[i + 1]);
		tmp->cmd_right->mode_open = 1;
	}
	if (ft_strcmp(tab[i], "<") == 0)
	{
		tmp->cmd_right->name_in = ft_strdup(tab[i + 1]);
		tmp->cmd_right->mode_open = 1;
		tmp->cmd_right->cmd = recalculcmd(tmp->cmd_right->cmd,
				tmp->cmd_right->name_in, "<");
	}
	return (tmp);
}

void	ouvertureredir(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		i;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	i = 0;
	while (tmp)
	{
		if (tmp->cmd_right && tmp->cmd_right->cmd)
		{
			tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
			i = 0;
			while (tab && tab[i])
			{
				tmp = cut_ouvertureredir(tmp, tab, i);
				tmp = cut_ouvertureredir_2(tmp, tab, i);
				tmp = cut_ouvertureredir_3(tmp, tab, i);
				i++;
			}
			free_tab(tab);
		}
		tmp = tmp->next;
	}
}
