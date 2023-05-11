/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:08:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 13:19:21 by jtaravel         ###   ########.fr       */
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
	if (tmp->pid == 0)
	{
		if (tmp->fd_in == -1)
		{
			free_all(recup_struct_env2(NULL, 2),
				recup_struct_env2(NULL, 6), s);
			cut_middle_execute_free(envtab, exectab);
			exit (1);
		}
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (!tmp->cmd || execve(tmp->cmd, exectab, envtab) == -1)
			cut_e_one_f_2(tmp, s, envtab, exectab);
		exit(0);
	}
	else
	{
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
	cut_middle_execute_free(envtab, exectab);
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

void	cut_executone(t_cmd *tmp, t_shell *shell, t_env **env)
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
		return (cut_exec_one_b(tmp, envtab, exectab, shell));
	if (tmp->cmd)
		del_sq_dq_arg(exectab);
	g_rvalue = 0;
	tmp->pid = fork();
	cut_execone_fork(tmp, envtab, exectab, shell);
}

void	executeone(t_cmd **cmd, t_env **env, t_shell *shell, t_env **exp)
{
	t_cmd	*tmp;

	(void)exp;
	tmp = *cmd;
	if (!tmp)
		return ;
	if (tmp->is_hd && !tmp->cmd && ft_strcmp(tmp->limiter, tmp->name_in) == 0)
	{
		tmp->pid = 0;
		return ;
	}
	if (!tmp->cmd)
		exec_fail(NULL);
	tmp = cut_exec_one_in_out(tmp);
	cut_executone(tmp, shell, env);
	if (tmp->fd_in == -1)
	{
		putstr_fd_echo("minishell: ", 2);
		putstr_fd_echo(tmp->name_in, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		g_rvalue = 1;
	}
}
