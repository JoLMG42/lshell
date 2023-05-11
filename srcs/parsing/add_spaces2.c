/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_spaces2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:20:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 17:49:21 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_space_malloc(char *str)
{
	int		i;
	int		c;
	char	*res;

	i = 0;
	c = 0;
	while (str[i])
	{
		if ((str[i] == '|' && str[i + 1] == '|')
			|| (str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '<' && str[i + 1] == '<')
			|| (str[i] == '>' && str[i + 1] == '>'))
		{
			c += 2;
			i++;
		}
		else if (str[i] == '|' || str[i] == '<'
			|| str[i] == '>' || str[i] == '(' || str[i] == ')')
			c += 1;
		i++;
	}
	res = malloc(sizeof(char *) * (ft_strlen(str) + c + 1));
	return (res);
}

char	*cut_add_space_while3(char *res, char *str, int i, int j)
{
	res[j] = ' ';
	j++;
	res[j] = str[i];
	j++;
	res[j] = ' ';
	j++;
	i++;
	return (res);
}

char	*cut_add_space_while2(char *res, char *str, int i, int j)
{
	res[j] = ' ';
	j++;
	res[j] = str[i];
	j++;
	res[j] = ' ';
	j++;
	i++;
	return (res);
}

char	*cut_add_space_while(char *res, char *str, int i, int j)
{
	res[j] = ' ';
	j++;
	res[j] = str[i];
	i++;
	j++;
	res[j] = str[i];
	i++;
	j++;
	res[j] = ' ';
	j++;
	return (res);
}

int	add_space_quote(int in_quote, char *str, int i)
{
	if ((str[i] == '\'' || str[i] == '\"') && in_quote == 0)
		in_quote++;
	else if ((str[i] == '\'' || str[i] == '\"') && in_quote)
		in_quote--;
	return (in_quote);
}
