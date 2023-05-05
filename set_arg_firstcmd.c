/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_arg_firstcmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:43:57 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 12:59:21 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_setargfirstcmd_2(t_tree *tmp, char **tab, int n)
{
	while (tab[++n])
	{
		if (tab[n][0] == '>')
		{
			while (tab[n])
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[n]);
				n++;
			}
			break ;
		}
		if (tab[0][0] == '<' || tab[n][0] == '<')
		{
			n = 0;
			while (tab[n])
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[n]);
				n++;
			}
			break ;
		}
	}
	return (tmp);
}

t_tree	*cut_setargfirstcmd(t_tree *tmp, t_s *s, char **tab)
{
	while (tab[s->i])
	{
		if (tab[s->i][0] == '>')
			break ;
		if (tab[s->i][0] == '<' || tab[0][0] == '<')
		{
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = NULL;
			break ;
		}
		tmp->cmd_left->arg[s->j] = ft_strdup(tab[s->i]);
		s->j++;
		s->i++;
	}
	return (tmp);
}

void	setargfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	t_s		s;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	init_syntax_struct(&s);
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	tmp->cmd_left->arg = malloc(sizeof(char *) * (tab_len(tab) + 1));
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = ft_strdup(tab[0]);
	s.i = 1;
	tmp = cut_setargfirstcmd(tmp, &s, tab);
	tmp = cut_setargfirstcmd_2(tmp, tab, 0);
	tmp->cmd_left->arg[s.j] = 0;
	free_tab(tab);
}
