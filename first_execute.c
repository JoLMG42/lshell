/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:38:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 18:07:42 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*middle_exec_in_out(t_cmd *tmp, t_shell *tree)
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
		tmp->fd_out = tree->pipefd[1];
	if (tmp->name_in)
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	return (tmp);
}

void	cut_first_execute2(t_cmd *tmp, char **et, char **ext, t_shell *t)
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
		exit(127);
	}
}

void	cut_first_execute(t_cmd *tmp, t_shell *tree, t_env **env)
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
		exectab = fusioncmdarg(tmp->cmd, tmp->arg);
		envtab = list_to_tab(env);
	}
	tmp = middle_exec_in_out(tmp, tree);
	if (tmp->cmd)
		del_sq_dq_arg(exectab);
	close(tree->pipefd[0]);
	if (dup2(tmp->fd_in, 0) == -1)
		perror("dup2");
	if (dup2(tmp->fd_out, 1) == -1)
		perror("dup2");
	cut_first_execute2(tmp, envtab, exectab, tree);
	exit(0);
}

void	first_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp)
{
	t_cmd	*tmp;
	int		frk;

	tmp = *cmd;
	if (!tmp->cmd)
		return ;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		cut_first_execute(tmp, tree, env);
	}
	else
	{
		close(tree->pipefd[1]);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
	return ;
}
