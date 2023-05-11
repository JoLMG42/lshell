/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_brace_lvl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:19:23 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 11:33:08 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_setbracelvl(t_tree *tmp, char **tab, t_s *s, int in_brace)
{
	if (ft_strcmp(tab[s->i], "(") == 0)
	{
		s->j = s->i + 1;
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = NULL;
		tmp->cmd_right->bracelvl = 1;
		in_brace++;
		while (tab[s->j] && in_brace)
		{
			if (ft_strcmp(tab[s->j], "(") == 0)
				in_brace++;
			if (ft_strcmp(tab[s->j], ")") == 0)
				in_brace--;
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[s->j]);
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
			s->j++;
			if (ft_strcmp(tab[s->j], ")") == 0 && in_brace == 1)
				break ;
			s->i++;
		}
	}
	return (tmp);
}

void	setbracelvl(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		in_brace;
	t_s		s;

	init_syntax_struct(&s);
	tmp = (*lst)->next;
	if (!tmp)
		return ;
	tab = NULL;
	while (tmp)
	{
		if (!tmp->cmd_right->cmd)
			return ;
		tab = ft_supersplit2(tmp->cmd_right->cmd, ' ');
		s.i = 0;
		in_brace = 0;
		while (tab && tab[s.i])
		{
			cut_setbracelvl(tmp, tab, &s, in_brace);
			s.i++;
		}
		free_tab(tab);
		tmp = tmp->next;
	}
}
