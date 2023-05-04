/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:07:27 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 00:48:10 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_parsearg_2(t_tree *tmp, t_s *s, char **tab, char *str)
{
	t_env	*tmpenv;

	if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") == 0)
		tmp->cmd_right->arg[s->i] = ft_strjoin(tmp->cmd_right->arg[s->i], "$$");
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "?") == 0)
	{
		str = ft_itoa(g_rvalue);
		tmp->cmd_right->arg[s->i] = ft_strjoin(tmp->cmd_right->arg[s->i], str);
		free(str);
	}
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") != 0)
	{
		tmpenv = (recup_struct_env(NULL, 1))->next;
		while (tmpenv)
		{
			if (ft_strcmp(tmpenv->name, tab[s->j + 1]) == 0)
				tmp->cmd_right->arg[s->i]
					= ft_strjoin(tmp->cmd_right->arg[s->i], tmpenv->content);
			tmpenv = tmpenv->next;
		}
	}
	if (!tab[s->j + 1])
		tmp->cmd_right->arg[s->i]
			= ft_strjoin(tmp->cmd_right->arg[s->i], tab[s->j]);
	return (tmp);
}

t_tree	*cut_parsearg(t_tree *tmp, t_s *s, char **tab, char *str)
{
	tmp->cmd_right->arg[s->i] = addspacedol(tmp->cmd_right->arg[s->i]);
	tab = ft_supersplit(tmp->cmd_right->arg[s->i], ' ');
	free(tmp->cmd_right->arg[s->i]);
	tmp->cmd_right->arg[s->i] = NULL;
	s->j = -1;
	while (tab && tab[++s->j] && s->j <= tab_len(tab))
	{
		if (ft_strcmp(tab[s->j], "$") == 0)
		{
			s->braces = 1;
			tmp = cut_parsearg_2(tmp, s, tab, str);
			s->j++;
		}
		else
			tmp->cmd_right->arg[s->i]
				= ft_strjoin(tmp->cmd_right->arg[s->i], tab[s->j]);
		if (tab[s->j] && tab[s->j + 1] && s->braces == 0)
		{
			if (s->j > 0 && ft_strcmp(tab[s->j - 1], "$") != 0)
				tmp->cmd_right->arg[s->i]
					= ft_strjoin(tmp->cmd_right->arg[s->i], " ");
		}
	}
	free_tab(tab);
	return (tmp);
}

void	parsearg(t_tree **lst, t_env **env)
{
	t_tree	*tmp;
	t_s		s;
	char	**tab;
	char	*str;

	(void)env;
	tab = NULL;
	str = NULL;
	init_syntax_struct(&s);
	tmp = (*lst)->next;
	while (tmp)
	{
		s.i = -1;
		while (tmp->cmd_right && tmp->cmd_right->arg[++s.i])
		{
			tmp = cut_parsearg(tmp, &s, tab, str);
			s.braces = 0;
		}
		tmp = tmp->next;
	}
}
