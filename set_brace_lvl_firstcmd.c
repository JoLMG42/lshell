/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_brace_lvl_firstcmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:28:46 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 10:04:09 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*cut_setbracelvlfirstcmd(t_tree *tmp, char **tab, t_s *s, int in_brace)
{
	if (ft_strcmp(tab[s->i], "(") == 0)
	{
		s->j = s->i + 1;
		free(tmp->cmd_left->cmd);
		tmp->cmd_left->cmd = NULL;
		tmp->cmd_left->bracelvl = 1;
		in_brace++;
		while (tab[s->j])
		{
			if (ft_strcmp(tab[s->j], "(") == 0)
				in_brace++;
			if (ft_strcmp(tab[s->j], ")") == 0)
				in_brace--;
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[s->j]);
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
			s->j++;
			if (ft_strcmp(tab[s->j], ")") == 0 && in_brace == 1)
				break ;
			s->i++;
		}
	}
	return (tmp);
}

void	setbracelvlfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int		in_brace;
	t_s		s;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	if (!tmp->cmd_left || !tmp->cmd_left->cmd)
		return ;
	init_syntax_struct(&s);
	tab = ft_supersplit2(tmp->cmd_left->cmd, ' ');
	in_brace = 0;
	while (tab[s.i])
	{
		tmp = cut_setbracelvlfirstcmd(tmp, tab, &s, in_brace);
		s.i++;
	}
	free_tab(tab);
}
