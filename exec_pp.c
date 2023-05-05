/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:02:15 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 11:05:15 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec pipe + next (pipe) */

t_tree	*exec_pp_pp(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
	{
		first_execute(&tmp->cmd_left, env, shell, exp);
		shell->tmpfd = shell->pipefd[0];
		pipe(shell->pipefd);
	}
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
	{
		middle_execute(&tmp->cmd_right, env, shell, shell->tmpfd);
		to_wait = tmp;
	}
	return (to_wait);
}

/* exec pipe + next (|| / &&) */

void	exec_ou_ee(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	(void)to_wait;
	env = recup_struct_env2(NULL, 2);
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
		first_execute(&tmp->cmd_left, env, shell, exp);
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
		last_execute(&tmp->cmd_right, env, shell, exp);
	if (!tmp->cmd_left->bracelvl)
		ft_wait(&(tmp->cmd_left));
	if (!tmp->cmd_right->bracelvl)
		ft_wait(&(tmp->cmd_right));
}

/* exec pipe split */

t_tree	*exec_pp(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (ft_strcmp(tmp->next->ope, "|") == 0)
		to_wait = exec_pp_pp(tmp, to_wait, exp, shell);
	else
		exec_ou_ee(tmp, to_wait, exp, shell);
	return (to_wait);
}
