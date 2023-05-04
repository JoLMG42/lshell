/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:50:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 00:43:22 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_parse_cmd_3(t_tree *tmp, char **recup)
{
	int	i;

	i = 0;
	while (recup[i])
	{
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, recup[i]);
		i++;
		if (recup[i])
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
	}
	return (tmp);
}

t_tree	*cut_parsecmd_2(t_tree *tmp, t_s *s, char **tab, char *str)
{
	if (ft_strcmp(tab[s->j], "$") == 0)
	{
		s->braces = 1;
		tmp = cut_parsecmd(tmp, s, tab, str);
		s->j++;
		if (!tab[s->j] || !tab[s->j + 1])
			return (tmp);
	}
	else
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[s->j]);
	if (tab[s->j] && tab[s->j][0] != '$' && s->braces == 0)
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
	s->braces = 0;
	return (tmp);
}

t_tree	*cut_parsecmd_env(t_tree *tmp, t_s *s, char **tab, t_env *tmpenv)
{
	tmpenv = (recup_struct_env(NULL, 1))->next;
	while (tmpenv)
	{
		if (ft_strcmp(tmpenv->name, tab[s->j + 1]) == 0)
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd,
					tmpenv->content);
		tmpenv = tmpenv->next;
	}
	return (tmp);
}

t_tree	*cut_parsecmd(t_tree *tmp, t_s *s, char **tab, char *str)
{
	t_env	*tmpenv;

	tmpenv = NULL;
	if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") == 0)
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, "$$");
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "?") == 0)
	{
		str = ft_itoa(g_rvalue);
		if (s->j > 1)
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, str);
		free(str);
	}
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") != 0)
		tmp = cut_parsecmd_env(tmp, s, tab, tmpenv);
	else
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[s->j]);
	return (tmp);
}

void	parsecmd(t_tree **lst, char **tab, char **recup, char *str)
{
	t_tree	*tmp;
	t_s		s;

	init_syntax_struct(&s);
	tmp = (*lst)->next;
	while (tmp)
	{
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		recup = dup_tab(tab + 1);
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = ft_strdup(tab[0]);
		tmp->cmd_right->cmd = addspacedol(tmp->cmd_right->cmd);
		free_tab(tab);
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = NULL;
		s.j = -1;
		while (tab && tab[++s.j] && s.j <= tab_len(tab))
			cut_parsecmd_2(tmp, &s, tab, str);
		tmp = cut_parse_cmd_3(tmp, recup);
		free_tab(tab);
		free_tab(recup);
		tmp = tmp->next;
	}
}
