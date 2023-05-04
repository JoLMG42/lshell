/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:36:09 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:45:05 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_setarg_2(t_tree *tmp, char **tab, int n)
{
	while (tab[++n])
	{
		if (tab[n][0] == '>')
		{
			while (tab[n])
			{
				tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
				tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[n]);
				n++;
			}
			break ;
		}
		if (tab[0][0] == '<' || tab[n][0] == '<')
		{
			n = 0;
			while (tab[n])
			{
				tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
				tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[n]);
				n++;
			}
			break ;
		}
	}
	return (tmp);
}

t_tree	*cut_setarg(t_tree *tmp, t_s *s, char **tab)
{
	s->i = 1;
	s->j = 0;
	while (tab[s->i])
	{
		if (tab[s->i][0] == '>')
			break ;
		if (tab[s->i][0] == '<' || tab[0][0] == '<')
		{
			free(tmp->cmd_right->cmd);
			tmp->cmd_right->cmd = NULL;
			break ;
		}
		tmp->cmd_right->arg[s->j] = ft_strdup(tab[s->i]);
		s->j++;
		s->i++;
	}
	return (tmp);
}

void	setarg(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	t_s		s;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	init_syntax_struct(&s);
	while (tmp)
	{
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		tmp->cmd_right->arg = malloc(sizeof(char *) * (tab_len(tab) + 1));
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = ft_strdup(tab[0]);
		tmp = cut_setarg(tmp, &s, tab);
		tmp = cut_setarg_2(tmp, tab, -1);
		tmp->cmd_right->arg[s.j] = 0;
		tmp = tmp->next;
		free_tab(tab);
	}
}
