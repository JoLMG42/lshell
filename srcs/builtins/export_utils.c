/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:31:23 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 15:30:04 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_name(char *str)
{
	int	i;

	if (str[0] >= 48 && str[0] <= 57)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '_' || (str[i] >= 48 && str[i] <= 57)
			|| (str[i] >= 65 && str[i] <= 90)
			|| (str[i] >= 97 && str[i] <= 122)
			|| (str[i] == '+'))
			i++;
		else
			return (0);
	}
	return (1);
}

int	check_plus(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '+')
			return (1);
		i++;
	}
	return (0);
}

int	check_egal(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	len_egal(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i + 1);
}

char	*before_egal(char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen(str)));
	if (!res)
		return (NULL);
	while (str && str[i] && str[i] != '=')
	{
		res[j] = str[i];
		j++;
		i++;
	}
	res[j] = 0;
	return (res);
}
