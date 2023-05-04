/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pp_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:07:07 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 15:07:50 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec 2nd group pipe */

void	exec_sec_grp_pp(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	shell->tmpfd = shell->pipefd[0];
	pipe(shell->pipefd);
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
		middle_execute(&tmp->cmd_right, env, shell, shell->tmpfd);
}

/* exec 2nd group end pipe serie */

t_tree	*exec_sec_pp_end(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
	{
		last_execute(&tmp->cmd_right, env, shell, exp);
		ft_wait_all(tw, tmp, shell->first_to_wait);
		tw = NULL;
	}
	return (tw);
}
/* exec 2nd group pipe(2) */

t_tree	*little_cut_pp_2(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	shell->tmpfd = shell->pipefd[0];
	pipe(shell->pipefd);
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
		middle_execute(&tmp->cmd_right, env, shell, shell->tmpfd);
	return (tw);
}

t_tree	*exec_sec_grp_pp_2(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
		tw = little_cut_pp_2(tmp, tw, exp, shell);
	else
	{
		if (tmp->cmd_right->bracelvl)
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
		else
		{
			last_execute(&tmp->cmd_right, env, shell, exp);
			if (!tw)
				tw = tmp;
			ft_wait_all(tw, tmp, shell->first_to_wait);
			tw = NULL;
		}
	}
	return (tw);
}
