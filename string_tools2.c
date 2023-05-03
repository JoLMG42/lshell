/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:12:51 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 17:18:13 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	check_sq_dq(t_s *s_s, char c)
{
	int	cop;

	cop = 0;
	if (c == '\'' && s_s->dq_opened == 0)
	{
		if (s_s->sq_opened == 0)
			s_s->sq_opened = 1;
		else
			s_s->sq_opened = 0;
	}
	if (c == '"' && s_s->sq_opened == 0)
	{
		if (s_s->dq_opened == 0)
			s_s->dq_opened = 1;
		else
			s_s->dq_opened = 0;
	}
	if (c == '(')
	{
		cop++;
		s_s->braces++;
	}
	if (c == ')' && s_s->braces)
	{
		cop--;
		s_s->braces--;
	}
}

t_env	**recup_struct_env2(t_env **env, int mode)
{
	static t_env	**tmp;
	static t_env	**tmpexp;

	if (mode == 1)
		tmp = env;
	if (mode == 2)
		return (tmp);
	if (mode == 5)
		tmpexp = env;
	if (mode == 6)
		return (tmpexp);
	return (NULL);
}
