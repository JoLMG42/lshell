/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:50:25 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/19 23:29:33 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	unset2(char *str, t_env *tmp)
{
	t_env	*tempo;
	
	if (!str || !tmp || !tmp->next)
		return ;
	if (tmp->next->name && ft_strcmp(str, tmp->next->name) == 0)
	{
		if (tmp->next->name)
			free(tmp->next->name);
		if (tmp->next->content)
			free(tmp->next->content);
		if (tmp->next->line)
			free(tmp->next->line);
		tempo = tmp->next->next;
		if (tmp->next)
			free(tmp->next);
		tmp->next = tempo;
	}
}

void	ft_unset(char **tab, t_env **env)
{
	int	i;
	t_env	*tmp;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		tmp = (*env)->next;
		while (tmp)
		{
			unset2(tab[i], tmp);
			tmp = tmp->next;
		}
		i++;
	}
}
