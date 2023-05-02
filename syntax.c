/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:14:19 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/02 17:36:04 by jtaravel         ###   ########.fr       */
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
}

/*int	check_parse_sq_dq(char *str, char c)
{
	int	i;
	int	count;
	int	opo;
	int	in_opo;
	int	in_actu;

	opo = '\'';
	if (c == '\'')
		opo = '\"';
	i = 0;
	count = 0;
	in_opo = 0;
	in_actu = 0;
	while (str[i])
	{
		if (!in_opo_check(str[i], opo, in_opo, in_actu))
			in_opo = 0;
		else
			in_opo = 1;
		if (str[i] == c && !in_opo && !in_actu)
		{
			count++;
			in_actu = 1;
		}
		else if (str[i] == c && in_actu)
		{
			count++;
			in_actu = 0;
		}
		i++;
	}
	if (count % 2 != 0)
		return (0);
	return (1);
}*/

int	check_parse_sq_dq(char *str, char c)
{
	int	i;
	int	count;
	int	opo;
	int	in_opo;
	int	in_actu;

	opo = '\'';
	if (c == '\'')
		opo = '\"';
	i = 0;
	count = 0;
	in_opo = 0;
	in_actu = 0;
	while (str[i])
	{
		if (str[i] == opo && !in_opo && !in_actu)
			in_opo = 1;
		else if (str[i] == opo && in_opo)
			in_opo = 0;
		if (str[i] == c && !in_opo && !in_actu)
		{
			count++;
			in_actu = 1;

		}
		else if (str[i] == c && in_actu)
		{
			count++;
			in_actu = 0;
		}
		i++;
	}
	if (count % 2 != 0)
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
