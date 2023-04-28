/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:22:42 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 17:48:45 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_spaces_check_syntax(char *str)
{
	if (!check_brace(str))
		return (0);
	if (!check_parse_sq_dq(str, '\''))
		return (0);
	if (!check_parse_sq_dq(str, '\"'))
		return (0);
	return (1);
}

int	first_condition(char *str, int i, int in_quote)
{
	if ((str[i] == '|' && str[i + 1] == '|')
		|| (str[i] == '&' && str[i + 1] == '&')
		|| (str[i] == '<' && str[i + 1] == '<')
		|| (str[i] == '>' && str[i + 1] == '>')
		&& in_quote == 0)
		return (1);
	return (0);
}

char	*add_space_all_while(char *str, char *res, t_s *s)
{
	s->in_quote = add_space_quote(s->in_quote, str, s->i);
	if (first_condition(str, s->i, s->in_quote))
	{
		res = cut_add_space_while(res, str, s->i, s->j);
		s->j += 4;
		s->i += 2;
	}
	else if ((str[s->i] == '|' || str[s->i] == '<'
			|| str[s->i] == '>') && s->in_quote == 0)
	{
		res = cut_add_space_while2(res, str, s->i, s->j);
		s->j += 3;
		s->i++;
	}
	else if ((str[s->i] == '(' || str[s->i] == ')') && s->in_quote == 0)
	{
		res = cut_add_space_while3(res, str, s->i, s->j);
		s->j += 3;
		s->i++;
	}
	else
		res[s->j++] = str[s->i++];
	return (res);
}

char	*add_spaces(char *str, int i, int j)
{
	char	*res;
	t_s		s;

	init_syntax_struct(&s);
	res = add_space_malloc(str);
	while (str && str[s.i])
	{
		add_space_all_while(str, res, &s);
	}
	if (!add_spaces_check_syntax(str))
	{
		free(res);
		return (0);
	}
	res[s.j] = 0;
	return (res);
}
