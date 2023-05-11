/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:13:37 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 17:40:22 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}

void	ft_lstadd_back_env(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = *alst;
	if ((*alst))
	{
		tmp = ft_lstlast_env(*alst);
		tmp->next = new;
	}
	else
	{
		*alst = new;
	}
}

void	ft_lstdelone_env(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst->line)
	{
		free(lst->line);
		lst->line = NULL;
	}
	if (lst->name)
	{
		free(lst->name);
		lst->name = NULL;
	}
	if (lst->content)
	{
		free(lst->content);
		lst->content = NULL;
	}
	free(lst);
	lst = NULL;
}

void	ft_lstclear_env(t_env **lst, void (*del)(void *))
{
	t_env	*list;
	t_env	*tmp;

	if (!lst || !del)
		return ;
	list = *lst;
	while (list)
	{
		tmp = list->next;
		ft_lstdelone_env(list, del);
		list = tmp;
	}
	*lst = NULL;
}

t_env	*ft_lstnew_env(char *line, char *name, char *content)
{
	t_env	*tmp;

	tmp = malloc(sizeof(struct s_env));
	if (!tmp)
		return (0);
	tmp->line = line;
	tmp->name = name;
	tmp->content = content;
	tmp->next = NULL;
	return (tmp);
}
