/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ou.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:06:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 16:21:26 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec || cmd_left */

void	exec_ou_left(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_left->bracelvl)
		braces_enginer(tmp->cmd_left->cmd, env, exp);
	else
	{
		exec_and(&tmp->cmd_left, env, exp, shell);
		ft_wait(&(tmp->cmd_left));
	}
}

/* exec || cmd_right */

t_tree	*exec_ou_right(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (ft_strcmp(tmp->next->ope, "|") == 0)
	{
		if (tmp->cmd_right->bracelvl)
			braces_enginer(tmp->cmd_right->cmd, env, exp);
		else
		{
			first_execute(&tmp->cmd_right, env, shell, exp);
			tw = tmp;
			shell->first_to_wait = 0;
		}
	}
	else
	{
		if (tmp->cmd_right->bracelvl)
			braces_enginer(tmp->cmd_right->cmd, env, exp);
		else
		{
			exec_and(&tmp->cmd_right, env, exp, shell);
			ft_wait(&(tmp->cmd_right));
		}
	}
	return (tw);
}

/* exec || split */

t_tree	*exec_ou(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	exec_ou_left(tmp, env, exp, shell);
	if (g_rvalue != 0)
		to_wait = exec_ou_right(tmp, to_wait, exp, shell);
	else
		shell->skip = 1;
	return (to_wait);
}
