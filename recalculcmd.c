/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recalculcmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:01:09 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/02 18:10:44 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*recalculcmd(char *cmd, char *str, char *ope)
{
	char	**tab;
	char	*res;
	int	i;

	res = NULL;
	if (ft_strcmp(ope, "<") == 0)
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

