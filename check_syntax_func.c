/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:24:55 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 10:56:22 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cut_check_syntax(char **tab, int i)
{
	if (ft_strcmp(tab[i], "||") == 0)
	{
		if (ft_strcmp(tab[i + 1], "|") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "|") == 0)
	{
		if (ft_strcmp(tab[i + 1], "||") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "|") == 0)
	{
		if (ft_strcmp(tab[i + 1], "&&") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "&&") == 0)
	{
		if (ft_strcmp(tab[i + 1], "|") == 0)
			return (0);
	}
	return (1);
}

int	cut_check_syntax_2(char **tab, int i)
{
	if (ft_strcmp(tab[i], "||") == 0)
	{
		if (ft_strcmp(tab[i + 1], "&&") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "&&") == 0)
	{
		if (ft_strcmp(tab[i + 1], "||") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "|") == 0)
	{
		if (ft_strcmp(tab[i + 1], "|") == 0)
			return (0);
	}
	if (ft_strcmp(tab[i], "&&") == 0)
	{
		if (ft_strcmp(tab[i + 1], "&&") == 0)
			return (0);
	}
	return (1);
}

int	cut_check_syntax_3(char **tab, int i)
{
	if (!cut_check_syntax(tab, i))
		return (0);
	if (!cut_check_syntax_2(tab, i))
		return (0);
	if (!check_brace(tab[i]))
		return (0);
	if (!check_parse_sq_dq(tab[i], '\''))
		return (0);
	if (!check_parse_sq_dq(tab[i], '\"'))
		return (0);
	return (1);
}

int	check_syntax(char **tab)
{
	int	i;
	int	j;
	int	sq;
	int	dq;

	i = -1;
	sq = 0;
	dq = 0;
	while (tab[++i])
	{
		if ((ft_strcmp(tab[i], "||") == 0 || ft_strcmp(tab[i], "|") == 0
				|| ft_strcmp(tab[i], "<") == 0
				|| ft_strcmp(tab[i], "&&") == 0 || ft_strcmp(tab[i], ">") == 0
				|| ft_strcmp(tab[i], "<<") == 0
				|| ft_strcmp(tab[i], ">>") == 0) && !tab[i + 1])
			return (0);
		if (!cut_check_syntax_3(tab, i))
			return (0);
	}
	if (dq == 1 || sq == 1)
		return (0);
	return (1);
}
