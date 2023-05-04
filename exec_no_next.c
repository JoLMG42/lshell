/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_no_next.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:33:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 14:33:41 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_one_only(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_right->cmd == NULL)
	{
		if (tmp->cmd_left->bracelvl)
			pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
		else
		{
			executeone(&tmp->cmd_left, env, shell, exp);
			ft_wait(&(tmp->cmd_left));
		}
		if (tmp->cmd_left->is_hd)
		{
			if (tmp->cmd_left->name_in)
				unlink(tmp->cmd_left->name_in);
		}
	}
}

/* exec no next pipe */

void	e_no_next_pp(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	char	*ttt;

	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
		first_execute(&tmp->cmd_left, env, shell, exp);
	if (tmp->cmd_right->bracelvl)
	{
		ttt = ft_strdup(tmp->cmd_right->cmd);
		pars_prompt(ttt, *env, *exp, 2);
		free(ttt);
	}
	else
	{
		last_execute(&tmp->cmd_right, env, shell, exp);
		ft_wait(&(tmp->cmd_left));
		ft_wait(&(tmp->cmd_right));
	}
}

/* exec no next && */

void	e_no_next_ee(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
	{
		exec_and(&tmp->cmd_left, env, exp, shell);
		ft_wait(&(tmp->cmd_left));
	}
	if (g_rvalue == 0)
	{
		if (tmp->cmd_right->bracelvl)
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
		else
		{
			exec_and(&tmp->cmd_right, env, exp, shell);
			ft_wait(&(tmp->cmd_right));
		}
	}
}

/* exec no next || */

void	e_no_next_ou(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
	else
	{
		exec_and(&tmp->cmd_left, env, exp, shell);
		ft_wait(&(tmp->cmd_left));
	}
	if (g_rvalue != 0)
	{
		if (tmp->cmd_right->bracelvl)
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
		else
		{
			exec_and(&tmp->cmd_right, env, exp, shell);
			ft_wait(&(tmp->cmd_right));
		}
	}
}

void	exec_no_next(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (ft_strcmp(tmp->ope, "|") == 0)
		e_no_next_pp(tmp, env, exp, shell);
	else if (ft_strcmp(tmp->ope, "&&") == 0)
		e_no_next_ee(tmp, env, exp, shell);
	else if (ft_strcmp(tmp->ope, "||") == 0)
		e_no_next_ou(tmp, env, exp, shell);
}
