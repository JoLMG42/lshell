/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:42:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/25 00:43:00 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_backtree(t_tree **alst, t_tree *new)
{
	t_tree	*tmp;

	tmp = *alst;
	if ((*alst))
	{
		tmp = ft_lstlasttree(*alst);
		tmp->next = new;
	}
	else
	{
		*alst = new;
	}
}

t_tree	*ft_lstlasttree(t_tree *lst)
{
	t_tree	*tmp;

	if (!lst)
		return lst;
	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}

void	ft_lstcleartree(t_tree **lst, void (*del)(void *))
{
	t_tree	*list;
	t_tree	*tmp;

	if (!lst || !del)
		return ;
	list = *lst;
	while (list)
	{
		tmp = list->next;
		ft_lstdelonetree(list, del);
		list = tmp;
	}
	*lst = NULL;
}

void	ft_lstdelonetree(t_tree *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst->cmd_left)
		ft_lstclear(&lst->cmd_left, del);
	if (lst->cmd_right)
		ft_lstclear(&lst->cmd_right, del);
	if (lst->ope)
		free(lst->ope);
	free(lst);
}

t_tree	*ft_lstnewtree(char *ope, t_cmd *cmdl, t_cmd *cmdr)
{
	t_tree	*tmp;

	tmp = malloc(sizeof(struct s_tree));
	if (!tmp)
		return (0);
	tmp->ope = ft_strdup(ope);
	tmp->cmd_left = cmdl;
	tmp->cmd_right = cmdr;
	tmp->in_exec = 0;
	tmp->next = NULL;
	return (tmp);
}
