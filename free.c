/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 18:58:24 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 18:00:08 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tree;

	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
	free(shell->saveope);
	free(shell);
	tree = recup_struct(NULL, 1);
	ft_lstcleartree(&tree, del);
}

void	free_stuff(t_env **env, t_env **exp)
{
	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
}

t_tree	*dup_tree(t_tree **adup)
{
	t_tree	*tmp;
	t_tree	*ret;

	tmp = (*adup);
	ret = malloc(sizeof(struct s_tree));
	ret = ft_lstnewtree((tmp->ope),
			ft_lstnew(tmp->cmd_left->cmd), ft_lstnew(tmp->cmd_right->cmd));
	tmp = tmp->next;
	while (tmp)
	{
		ft_lstadd_backtree(&ret, ft_lstnewtree((tmp->ope),
				ft_lstnew(tmp->cmd_left->cmd), ft_lstnew(tmp->cmd_right->cmd)));
		tmp = tmp->next;
	}
	return (ret);
}

int	*recup_flag(int *i)
{
	static int	*tmp;

	if (i)
		tmp = i;
	if (!i)
		return (tmp);
	return (0);
}
