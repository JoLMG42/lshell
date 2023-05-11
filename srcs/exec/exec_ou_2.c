/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ou_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:11:06 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 16:21:41 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exec 2nd group || (cut function) */

t_tree	*ex_sec_grp_ou(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
		tw = little_cut(tmp, tw, exp, shell);
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

/* exec 2nd group || */

t_tree	*exec_sec_grp_ou(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	if (g_rvalue != 0)
		tw = ex_sec_grp_ou(tmp, tw, exp, shell);
	else
		shell->skip = 1;
	return (tw);
}
