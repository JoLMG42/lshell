/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_tools4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:40:11 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 17:15:57 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	listenv_size(t_env **env)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = (*env)->next;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**list_to_tab(t_env **env)
{
	t_env	*tmp;
	int		i;
	char	**res;

	tmp = (*env)->next;
	res = malloc(sizeof(char *) * (listenv_size(env) + 1));
	i = 0;
	while (tmp)
	{
		res[i] = ft_strdup(tmp->line);
		i++;
		tmp = tmp->next;
	}
	res[i] = 0;
	return (res);
}

t_tree	*recup_struct(t_tree **tree, int mode)
{
	static t_tree	*tmp;
	static t_tree	*toto;

	if (mode == 3)
		tmp = NULL;
	if (tree != NULL)
		tmp = *tree;
	if (tree == NULL && tmp && mode == 2)
		return (tmp->next);
	if (tree == NULL && tmp && mode == 1)
		return (tmp);
	if (mode == 9)
		toto = *tree;
	if (mode == 10)
		return (toto);
	return (NULL);
}

t_env	*recup_struct_env(t_env **env, int mode)
{
	static t_env	*tmp;
	static t_env	*tmpexp;

	if (mode == 0)
		tmp = *env;
	if (mode == 1)
		return (tmp);
	if (mode == 5)
		tmpexp = *env;
	if (mode == 6)
		return (tmpexp);
	return (NULL);
}

void	del_sq_dq_arg(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		ft_suppr_dq_sq(tab[i]);
		i++;
	}
}
