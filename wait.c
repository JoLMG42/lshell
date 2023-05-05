/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:44:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 18:45:03 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait(t_cmd **cmd)
{
	int		value;
	t_cmd	*cmd_lst;

	value = 0;
	cmd_lst = *cmd;
	if (!cmd_lst)
		return ;
	if (cmd_lst->cmd && is_builtins(cmd_lst->cmd))
		return ;
	waitpid(cmd_lst->pid, &value, 0);
	if (WIFSIGNALED(value))
		g_rvalue = (WTERMSIG(value) + 128);
	else
		g_rvalue = WEXITSTATUS(value);
}

void	ft_wait_all(t_tree *to_wait, t_tree *end, int first)
{
	t_tree	*tmp;

	tmp = to_wait;
	while (tmp != end)
	{
		if (tmp == to_wait && first)
			ft_wait(&(tmp->cmd_left));
		ft_wait(&(tmp->cmd_right));
		tmp = tmp->next;
	}
	if (tmp == to_wait && first)
		ft_wait(&(tmp->cmd_left));
	ft_wait(&(tmp->cmd_right));
}
