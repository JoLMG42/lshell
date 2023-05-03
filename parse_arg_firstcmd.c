/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg_firstcmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:42:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 15:05:45 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_parseargfirstcmd_3(t_tree *tmp, char **tab, t_s *s, char *str)
{
	if (ft_strcmp(tab[s->j], "$") == 0)
		tmp = cut_parseargfirstcmd_2(tmp, tab, s, str);
	else
		tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i],
				tab[s->j]);
	if (tab[s->j] && tab[s->j + 1])
		tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i], " ");
	return (tmp);
}

t_tree	*cut_parseargfirstcmd_2(t_tree *tmp, char **tab, t_s *s, char *str)
{
	tmp = cut_parseargfirstcmd(tmp, tab, s, str);
	if (!tab[s->j + 1])
		tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i],
				tab[s->j]);
	s->j++;
	return (tmp);
}

t_tree	*cut_parseargfirstcmd(t_tree *tmp, char **tab, t_s *s, char *str)
{
	t_env	*tmpenv;

	s->braces = 1;
	if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") == 0)
		tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i], "$$");
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "?") == 0)
	{
		str = ft_itoa(g_rvalue);
		tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i], str);
		free(str);
	}
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") != 0)
	{
		tmpenv = (recup_struct_env(NULL, 1))->next;
		while (tmpenv)
		{
			if (ft_strcmp(tmpenv->name, tab[s->j + 1]) == 0)
				tmp->cmd_left->arg[s->i] = ft_strjoin(tmp->cmd_left->arg[s->i],
						tmpenv->content);
			tmpenv = tmpenv->next;
		}
	}
	return (tmp);
}

void	parseargfirstcmd(t_tree **lst, t_env **env)
{
	t_tree	*tmp;
	char	**tab;
	char	*str;
	t_s		s;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->arg)
		return ;
	init_syntax_struct(&s);
	s.i = -1;
	while (tmp->cmd_left->arg[++s.i])
	{
		tmp->cmd_left->arg[s.i] = addspacedol(tmp->cmd_left->arg[s.i]);
		tab = ft_supersplit(tmp->cmd_left->arg[s.i], ' ');
		free(tmp->cmd_left->arg[s.i]);
		tmp->cmd_left->arg[s.i] = NULL;
		s.j = -1;
		while (tab[++s.j] && s.j <= tab_len(tab))
		{
			tmp = cut_parseargfirstcmd_3(tmp, tab, &s, str);
		}
		s.braces = 0;
		free_tab(tab);
	}
}
