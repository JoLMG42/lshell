/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:21:15 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 14:29:10 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*check_middle_in_out(t_cmd *tmp, t_shell *tree, int fd_temp)
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
	else
	{
		tmp->fd_out = tree->pipefd[1];
		close(tree->pipefd[0]);
	}
	if (tmp->name_in)
	{
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	}
	else
		tmp->fd_in = fd_temp;
	return (tmp);
}

void	cut_middle_execute_free(char **et, char **ext)
{
	free_tab(et);
	free_tab(ext);
}

void	cut_middle_execute_2(t_cmd *tmp, char **et, char **ext, t_shell *t)
{
	if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
	{
		tmp->pid = 0;
		cut_middle_execute_free(et, ext);
	}
	if (tmp->cmd && check_builtins(tmp, recup_struct_env2(NULL, 2),
			recup_struct_env2(NULL, 6), t))
	{
		tmp->pid = 0;
		if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
			return ;
		free_all(recup_struct_env2(NULL, 2),
			recup_struct_env2(NULL, 6), t);
		cut_middle_execute_free(et, ext);
		exit(0);
	}
	else if (!tmp->cmd || execve(tmp->cmd, ext, et) == -1)
	{
		if (tmp->cmd)
			check_slash(tmp->cmd, 1);
		free_all(recup_struct_env2(NULL, 2),
			recup_struct_env2(NULL, 6), t);
		cut_middle_execute_free(et, ext);
		exit(127);
	}
}

void	cut_middle_execute(t_cmd *tmp, t_shell *tree, t_env **env, int fd_temp)
{
	char	**envtab;
	char	**exectab;

	envtab = NULL;
	exectab = NULL;
	tmp = check_middle_in_out(tmp, tree, fd_temp);
	if (tmp->fd_in == -1)
		cut_middle_error(tmp->name_in, tree);
	if (tmp->cmd)
	{
		ft_suppr_dq_sq(tmp->cmd);
		tmp->cmd = reparse_dol(tmp->cmd, env);
		tmp->cmd = recup_cmd(tmp->cmd, env, 0);
		exectab = fusioncmdarg(tmp->cmd, tmp->arg);
		envtab = list_to_tab(env);
	}
	if (tmp->cmd)
		del_sq_dq_arg(exectab);
	if (dup2(tmp->fd_in, 0) == -1)
		perror("dup2");
	if (dup2(tmp->fd_out, 1) == -1)
		perror("dup2");
	cut_middle_execute_2(tmp, envtab, exectab, tree);
	exit(0);
}

void	middle_execute(t_cmd **cmd, t_env **env, t_shell *t, int f)
{
	t_cmd	*tmp;

	tmp = *cmd;
	if (!tmp)
		return ;
	if (tmp->cmd)
		tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
		cut_middle_execute(tmp, t, env, f);
	else
	{
		close(t->pipefd[1]);
		if (f != 0)
			close(f);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}
