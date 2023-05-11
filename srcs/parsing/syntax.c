/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:14:19 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:14:06 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_syntax_struct(t_s *s)
{
	s->dq_opened = 0;
	s->sq_opened = 0;
	s->braces = 0;
	s->in_quote = 0;
	s->i = 0;
	s->j = 0;
}

int	in_opo_check(char c, int opo, int in_opo, int in_actu)
{
	if (c == opo && !in_opo && !in_actu)
		return (1);
	else if (c == opo && in_opo)
		return (0);
	return (0);
}

void	cut_check_parse_sq_dq(t_s *s, int opo, char c, char *str)
{
	(void)c;
	if (str[s->i] == opo && !s->dq_opened && !s->sq_opened)
		s->dq_opened = 1;
	else if (str[s->i] == opo && s->dq_opened)
		s->dq_opened = 0;
}

int	check_parse_sq_dq(char *str, char c)
{
	t_s	s;
	int	opo;

	opo = '\'';
	if (c == '\'')
		opo = '\"';
	init_syntax_struct(&s);
	while (str[s.i])
	{
		cut_check_parse_sq_dq(&s, opo, c, str);
		if (str[s.i] == c && !s.dq_opened && !s.sq_opened)
		{
			s.braces++;
			s.sq_opened = 1;
		}
		else if (str[s.i] == c && s.sq_opened)
		{
			s.braces++;
			s.sq_opened = 0;
		}
		s.i++;
	}
	if (s.braces % 2 != 0)
		return (0);
	return (1);
}

int	check_brace(char *str)
{
	int	i;
	int	in_brace;

	i = 0;
	in_brace = 0;
	while (str[i])
	{
		if (str[i] == '(')
			in_brace++;
		if (str[i] == ')')
			in_brace--;
		i++;
	}
	if (in_brace != 0)
		return (0);
	return (1);
}
