/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:42:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/12 19:14:41 by jtaravel         ###   ########.fr       */
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
	//if (lst->cmd)
	//	(*del)(lst->cmd);
	free(lst);
}

t_tree	*ft_lstnewtree(char *ope, char *cmdl, char *cmdr)
{
	t_tree	*tmp;

	tmp = malloc(sizeof(struct s_tree));
	if (!tmp)
		return (0);
	tmp->ope = ope;
	tmp->cmd_left = cmdl;
	tmp->cmd_right = cmdr;
	tmp->next = NULL;
	return (tmp);
}
