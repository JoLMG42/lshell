/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:11:53 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 10:47:57 by jtaravel         ###   ########.fr       */
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

void	init_heredoc(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;

	tmp = (*tree)->next;
	if (tmp && tmp->cmd_left->is_hd)
		heredoc(&tmp->cmd_left, env, exp, shell);
	while (tmp)
	{
		if (tmp->cmd_right->is_hd)
			heredoc(&tmp->cmd_right, env, exp, shell);
		tmp = tmp->next;
	}
}

void	init_hd(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp && tmp->cmd_left && tmp->cmd_left->is_hd)
		heredoc(&tmp->cmd_left, env, exp, shell);
	if (tmp && tmp->cmd_right && tmp->cmd_right->is_hd)
		heredoc(&tmp->cmd_right, env, exp, shell);
}
