/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:08:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 18:30:59 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cut_exec_one_in_out(t_cmd *tmp)
{
	if (tmp->name_out)
	{
		if (tmp->mode_open == 1)
			tmp->fd_out = open(tmp->name_out, O_CREAT
					| O_RDONLY | O_WRONLY | O_TRUNC, 0644);
		else if (tmp->mode_open == 2)
			tmp->fd_out = open(tmp->name_out, O_CREAT
					| O_RDONLY | O_WRONLY | O_APPEND, 0644);
	}
	if (tmp->name_in)
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	return (tmp);
}

void	cut_execone_fork(t_cmd *tmp, char **envtab, char **exectab, t_shell *s)
{
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (!tmp->cmd || execve(tmp->cmd, exectab, envtab) == -1)
		{
			if (tmp->cmd)
				check_slash(tmp->cmd, 1);
			free_all(recup_struct_env2(NULL, 2),
				recup_struct_env2(NULL, 6), s);
			cut_middle_execute_free(envtab, exectab);
			exit(127);
		}
		exit(0);
	}
	else
	{
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
	free_tab(exectab);
	free_tab(envtab);
}

void	cut_exec_one_b(t_cmd *tmp, char **envtab, char **exectab, t_shell *s)
{
	if (tmp->cmd)
	{
		if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
		{
			tmp->pid = 0;
			free_tab(envtab);
			free_tab(exectab);
		}
		if (check_builtins(tmp, recup_struct_env2(NULL, 2),
				recup_struct_env2(NULL, 6), s))
		{
			tmp->pid = 0;
			if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
				return ;
			free_tab(exectab);
			free_tab(envtab);
			return ;
		}
	}
}

void	cut_executone(t_cmd *tmp, t_shell *shell, t_env **env, t_env **exp)
{
	char	**envtab;
	char	**exectab;

	envtab = NULL;
	exectab = NULL;
	if (tmp->cmd)
	{
		ft_suppr_dq_sq(tmp->cmd);
		tmp->cmd = reparse_dol(tmp->cmd, env);
		tmp->cmd = recup_cmd(tmp->cmd, env, 0);
		if (tmp->cmd)
		{
			exectab = fusioncmdarg(tmp->cmd, tmp->arg);
			envtab = list_to_tab(env);
		}
	}
	if (tmp->cmd && is_builtins(tmp->cmd))
		cut_exec_one_b(tmp, envtab, exectab, shell);
	if (tmp->cmd)
		del_sq_dq_arg(exectab);
	g_rvalue = 0;
	cut_execone_fork(tmp, envtab, exectab, shell);
}

void	executeone(t_cmd **cmd, t_env **env, t_shell *shell, t_env **exp)
{
	t_cmd	*tmp;
	int		frk;

	tmp = *cmd;
	if (!tmp)
		return ;
	tmp = cut_exec_one_in_out(tmp);
	cut_executone(tmp, shell, env, exp);
}