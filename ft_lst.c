/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:42:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/17 13:09:19 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back(t_cmd **alst, t_cmd *new)
{
	t_cmd	*tmp;

	tmp = *alst;
	if ((*alst))
	{
		tmp = ft_lstlast(*alst);
		tmp->next = new;
	}
	else
	{
		*alst = new;
	}
}

void	del(void *data)
{
	free(data);
}

t_cmd	*ft_lstlast(t_cmd *lst)
{
	t_cmd	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}

void	ft_lstclear(t_cmd **lst, void (*del)(void *))
{
	t_cmd	*list;
	t_cmd	*tmp;

	if (!lst || !del)
		return ;
	list = *lst;
	while (list)
	{
		tmp = list->next;
		ft_lstdelone(list, del);
		list = tmp;
	}
	*lst = NULL;
}

void	ft_lstdelone(t_cmd *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst->cmd)
		free(lst->cmd);
	if (lst->name_in)
		free(lst->name_in);
	if (lst->name_out)
		free(lst->name_out);
	if (lst->limiter)
		free(lst->limiter);
	if (lst->arg)
		free_tab(lst->arg);
	lst->bracelvl = 0;
	lst->is_hd = 0;
	free(lst);
}

t_cmd	*ft_lstnew(char	*cmd)
{
	t_cmd	*tmp;

	tmp = malloc(sizeof(struct s_cmd));
	if (!tmp)
		return (0);
	tmp->cmd = cmd;
	tmp->name_in = NULL;
	tmp->name_out = NULL;
	tmp->limiter = NULL;
	tmp->next = NULL;
	tmp->arg = NULL;
	tmp->pos = 0;
	tmp->fd_in = 0;
	tmp->fd_out = 1;
	tmp->mode_open = 0;
	tmp->is_hd = 0;
	tmp->bracelvl = 0;
	return (tmp);
}
