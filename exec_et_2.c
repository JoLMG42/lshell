/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_et_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:08:58 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 15:10:32 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec 2nd group && (cut function) */

t_tree	*little_cut(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	pipe(shell->pipefd);
	if (tmp->cmd_right->bracelvl)
		pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
	else
	{
		first_execute(&tmp->cmd_right, env, shell, exp);
		tw = tmp;
		shell->first_to_wait = 0;
	}
	return (tw);
}

t_tree	*ex_sec_grp_ee(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
		tw = little_cut(tmp, tw, exp, shell);
	else
	{
		if (tmp->cmd_right->bracelvl)
			pars_prompt(ft_strdup(tmp->cmd_right->cmd), *env, *exp, 2);
		else
		{
			exec_and(&tmp->cmd_right, env, exp, shell);
			ft_wait(&(tmp->cmd_right));
		}
	}
	return (tw);
}

/* exec 2nd group && */

t_tree	*exec_sec_grp_ee(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	if (g_rvalue == 0)
		tw = ex_sec_grp_ee(tmp, tw, exp, shell);
	else
		shell->skip = 1;
	return (tw);
}
