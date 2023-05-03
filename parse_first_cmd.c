/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_first_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:20:29 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 15:49:58 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_parsfirstcmd_3(t_tree *tmp, t_s *s, char **recup)
{
	int	j;

	j = 0;
	while (recup[j])
	{
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, recup[j]);
		j++;
		if (recup[j])
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
	}
	return (tmp);
}

t_tree	*cut_parsfirstcmd_2(t_tree *tmp, t_s *s, char **tab, char *str)
{
	if (ft_strcmp(tab[s->j], "$") == 0)
	{
		s->braces = 1;
		tmp = cut_parsfirstcmd(tmp, s, tab, str);
		s->j++;
		if (!tab[s->j] || !tab[s->j + 1])
			return (tmp);
	}
	else
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[s->j]);
	if (tab[s->j] && tab[s->j][0] != '$' && s->braces == 0)
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
	s->braces = 0;
	return (tmp);
}

t_tree	*cut_parsefirstcmd_env(t_tree *tmp, char **tab, t_s *s, t_env *tmpenv)
{
	tmpenv = (recup_struct_env(NULL, 1))->next;
	while (tmpenv)
	{
		if (tmpenv->name && ft_strcmp(tmpenv->name, tab[s->j + 1]) == 0)
			tmp->cmd_left->cmd
				= ft_strjoin(tmp->cmd_left->cmd, tmpenv->content);
		tmpenv = tmpenv->next;
	}
	return (tmp);
}

t_tree	*cut_parsfirstcmd(t_tree *tmp, t_s *s, char **tab, char *str)
{
	t_env	*tmpenv;

	if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") == 0)
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, "$$");
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "?") == 0)
	{
		str = ft_itoa(g_rvalue);
		if (s->j > 1)
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, str);
		free(str);
	}
	else if (tab[s->j + 1] && ft_strcmp(tab[s->j + 1], "$") != 0)
		tmp = cut_parsefirstcmd_env(tmp, tab, s, tmpenv);
	else
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[s->j]);
	return (tmp);
}

void	parsefirstcmd(t_tree **lst, t_env **env, char *str)
{
	t_tree	*tmp;
	char	**tab;
	t_s		s;
	char	**recup;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	recup = dup_tab(tab + 1);
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = ft_strdup(tab[0]);
	tmp->cmd_left->cmd = addspacedol(tmp->cmd_left->cmd);
	free_tab(tab);
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = NULL;
	init_syntax_struct(&s);
	s.j = -1;
	while (tab && tab[++s.j] && s.j <= tab_len(tab))
		tmp = cut_parsfirstcmd_2(tmp, &s, tab, str);
	tmp = cut_parsfirstcmd_3(tmp, &s, recup);
	free_tab(recup);
	free_tab(tab);
}
