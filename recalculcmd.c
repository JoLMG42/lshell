/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recalculcmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:01:09 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 11:02:21 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cut_recalculcmd(char *cmd, char *ope, char *str)
{
	char	**tab;
	char	*res;
	int		i;

	tab = ft_supersplit(cmd, ' ');
	i = 0;
	while (tab[i])
	{
		if (str && ft_strcmp(tab[i], str) && ft_strcmp(tab[i], ope))
			res = ft_strdup(tab[i]);
		i++;
	}
	free_tab(tab);
	return (res);
}

char	*recalculcmd(char *cmd, char *str, char *ope)
{
	char	**tab;
	char	*res;
	int		i;

	res = NULL;
	if (ft_strcmp(ope, "<") == 0)
		res = cut_recalculcmd(cmd, ope, str);
	if (ft_strcmp(ope, "<<") == 0)
	{
		tab = ft_supersplit(cmd, ' ');
		i = 0;
		while (tab[i])
		{
			if (str && ft_strcmp(tab[i], str) && ft_strcmp(tab[i], ope))
				res = ft_strdup(tab[i]);
			i++;
		}
		free_tab(tab);
	}
	free(cmd);
	return (res);
}
