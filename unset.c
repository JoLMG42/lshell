/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:50:25 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 23:18:47 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	mini_unset(char *str, t_env *tmp)
{
	t_env *tempo;

	if (tmp->name)
		free(tmp->name);
	if (tmp->content)
		free(tmp->content);
	if (tmp->next->line)
		free(tmp->line);
	tempo = tmp->next;
	if (tmp->next)
		free(tmp->next);
	tmp= tempo;
}

int	unset2(char *str, t_env *tmp)
{
	t_env	*tempo;
	
	/*if (tmp && tmp->name && str && ft_strcmp(tmp->name, str) == 0 && !tmp->next)
	{
		ft_lstdelone_env(tmp, del);
		tmp = NULL;
		return (1);
	}*/
	if (!str || !tmp || !tmp->next)
		return (0);
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
	return (0);
}

void	ft_unset(char **tab, t_env **env, t_env **exp)
{
	int	i;
	t_env	*tmp;
	t_env	*tmpexp;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		tmp = (*env)->next;
		while (tmp)
		{
			//printf("TAb[i] = %s     name = %s\n", tab[i], tmp->name);
			if (unset2(tab[i], tmp) == 1)
				break ;
			tmp = tmp->next;
		}
		tmpexp = (*exp)->next;
		while (tmpexp)
		{
			if (unset2(tab[i], tmpexp) == 1)
				break ;
			tmpexp = tmpexp->next;
		}
		i++;
	}
}
