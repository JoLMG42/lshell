/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_et.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:03:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 15:05:21 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec && cmd_left */

void	exec_ee_left(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
	{
		exec_and(&tmp->cmd_left, env, exp, shell);
		ft_wait(&(tmp->cmd_left));
	}
}

/*exec && cmd_right */

t_tree	*exec_ee_right(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (ft_strcmp(tmp->next->ope, "|") == 0)
	{
		if (tmp->cmd_right->bracelvl)
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
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
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
		else
		{
			exec_and(&tmp->cmd_right, env, exp, shell);
			ft_wait(&(tmp->cmd_right));
		}
	}
	return (tw);
}

/* exec && split */

t_tree	*exec_ee(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	exec_ee_left(tmp, env, exp, shell);
	if (g_rvalue == 0)
		to_wait = exec_ee_right(tmp, to_wait, exp, shell);
	else
		shell->skip = 1;
	return (to_wait);
}
