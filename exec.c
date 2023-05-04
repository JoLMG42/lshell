/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 14:38:52 by jtaravel         ###   ########.fr       */
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

/* exec || cmd_left */

void	exec_ou_left(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell)
{
	if (tmp->cmd_left->bracelvl)
		pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
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
			pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
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

t_tree	*exec_first_pass(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (!tmp->next)
		exec_no_next(tmp, env, exp, shell);
	else if (ft_strcmp(tmp->ope, "|") == 0)
		tw = exec_pp(tmp, tw, exp, shell);
	else if (ft_strcmp(tmp->ope, "&&") == 0)
		tw = exec_ee(tmp, tw, exp, shell);
	else if (ft_strcmp(tmp->ope, "||") == 0)
		tw = exec_ou(tmp, tw, exp, shell);
	return (tw);
}

t_tree	*exec_after(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->next && ft_strcmp(shell->saveope, "|") == 0
		&& ft_strcmp(tmp->ope, "|") == 0
		&& ft_strcmp(tmp->next->ope, "|") == 0)
		exec_sec_grp_pp(tmp, env, exp, shell);
	else if (ft_strcmp(shell->saveope, "|") == 0
		&& ft_strcmp(tmp->ope, "|") == 0)
		to_wait = exec_sec_pp_end(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "&&") == 0)
		to_wait = exec_sec_grp_ee(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "||") == 0)
		to_wait = exec_sec_grp_ou(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "|") == 0)
		to_wait = exec_sec_grp_pp_2(tmp, to_wait, exp, shell);
	return (to_wait);
}

t_tree	*exec_end_while(t_tree *tmp, t_shell *shell)
{
	if (tmp && tmp->cmd_left->is_hd)
	{
		if (tmp->cmd_left->name_in)
			unlink(tmp->cmd_left->name_in);
	}
	if (tmp && tmp->cmd_right->is_hd)
	{
		if (tmp->cmd_right->name_in)
			unlink(tmp->cmd_right->name_in);
	}
	if (shell->saveope)
	{
		free(shell->saveope);
		shell->saveope = NULL;
	}
	shell->saveope = ft_strdup(tmp->ope);
	return (tmp);
}

void	exec(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;
	int		i;
	t_tree	*to_wait;
	t_tree	*toto;

	i = 0;
	init_heredoc(tree, env, exp, shell);
	tmp = (*tree)->next;
	pipe(shell->pipefd);
	(*tree)->next->in_exec = 1;
	signal(SIGINT, handler_fork);
	signal(SIGQUIT, handler_fork);
	to_wait = NULL;
	while (tmp)
	{
		if (!tmp->cmd_right->cmd)
		{
			exec_one_only(tmp, env, exp, shell);
			return ;
		}
		if (i == 0)
			to_wait = exec_first_pass(tmp, to_wait, exp, shell);
		else
			to_wait = exec_after(tmp, to_wait, exp, shell);
		tmp = skip_pipe(tmp, shell);
		i++;
		tmp = exec_end_while(tmp, shell);
		tmp = tmp->next;
	}
	if (shell && shell->saveope)
	{
		free(shell->saveope);
		shell->saveope = NULL;
	}
}
