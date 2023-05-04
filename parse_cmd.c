/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:50:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:11:46 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_parsecmd_2(t_tree *tmp, t_s *s, char **tab, char *str)
{
	if (ft_strcmp(tab[s->j], "$") == 0)
	{
		tmp = cut_parsecmd(tmp, s, tab, str);
		s->j++;
	}
	else
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[s->j]);
	tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
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
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, "999");
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
	if (!tab[s->j + 1])
		tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[s->j]);
	return (tmp);
}

void	parsecmd(t_tree **lst, t_env **env, char *str)
{
	t_tree	*tmp;
	char	**tab;
	t_s		s;

	(void)env;
	init_syntax_struct(&s);
	tmp = (*lst)->next;
	while (tmp)
	{
		tmp->cmd_right->cmd = addspacedol(tmp->cmd_right->cmd);
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = NULL;
		s.j = -1;
		while (tab && tab[++s.j] && s.j <= tab_len(tab))
			cut_parsecmd_2(tmp, &s, tab, str);
		free_tab(tab);
		tmp = tmp->next;
	}
}
