/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:11:53 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 13:40:40 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_fork(int sig)
{
	if (sig == 2)
	{
		g_rvalue = 130;
		printf("\n");
		return ;
	}
	if (sig == 3)
	{
		g_rvalue = 131;
		printf("Quit (core dumped)\n");
		return ;
	}
}

t_tree	*skip_pipe(t_tree *tmp, t_shell *shell)
{
	if (shell->skip == 1)
	{
		while (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
			tmp = tmp->next;
	}
	shell->skip = 0;
	return (tmp);
}

int	minicheck(t_cmd *tmp)
{
	if (g_rvalue == 130)
	{
		unlink(tmp->name_in);
		return (130);
	}
	return (0);
}

int	init_heredoc(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;

	tmp = (*tree)->next;
	if (tmp && tmp->cmd_left->is_hd)
	{
		heredoc(&tmp->cmd_left, env, exp, shell);
		if (minicheck(tmp->cmd_left) == 130)
			return (130);
	}
	while (tmp)
	{
		if (tmp->cmd_right->is_hd)
		{
			heredoc(&tmp->cmd_right, env, exp, shell);
			if (minicheck(tmp->cmd_right) == 130)
				return (130);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	init_hd(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp && tmp->cmd_left && tmp->cmd_left->is_hd)
		heredoc(&tmp->cmd_left, env, exp, shell);
	if (tmp && tmp->cmd_right && tmp->cmd_right->is_hd)
		heredoc(&tmp->cmd_right, env, exp, shell);
}
