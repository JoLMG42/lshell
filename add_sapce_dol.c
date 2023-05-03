/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sapce_dol.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:11:06 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 14:37:42 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cut_addspacedol(char *str, t_s *s, char *res, int i)
{
	while (str[i])
	{
		check_sq_dq(s, str[i]);
		if (str[i] == '$' && s->sq_opened == 0 && s->dq_opened == 0)
		{
			res[s->j] = ' ';
			s->j++;
			res[s->j] = str[i];
			i++;
			s->j++;
			res[s->j] = ' ';
			s->j++;
		}
		else
		{
			res[s->j] = str[i];
			s->j++;
			i++;
		}
	}
	return (res);
}

char	*addspacedol(char *str)
{
	char	*res;
	int		f;
	t_s		s_s;

	init_syntax_struct(&s_s);
	s_s.i = -1;
	f = 0;
	while (str[++s_s.i])
	{
		if (str[s_s.i] == '$')
		{
			s_s.braces += 2;
			f = 1;
		}
		else
			s_s.braces++;
	}
	if (f == 0)
		return (str);
	res = malloc(sizeof(char *) * (s_s.braces + 10));
	res = cut_addspacedol(str, &s_s, res, 0);
	free(str);
	res[s_s.j] = 0;
	return (res);
}
