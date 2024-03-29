/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:51:46 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 14:29:14 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*check_in_out(t_cmd *tmp, t_shell *tree)
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
	else
		tmp->fd_in = tree->pipefd[0];
	if (tmp->fd_in == -1)
	{
		putstr_fd_echo("minishell: ", 2);
		putstr_fd_echo(tmp->name_in, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		free_all(recup_struct_env2(NULL, 2), recup_struct_env2(NULL, 6), tree);
		exit(1);
	}
	return (tmp);
}

void	cut_last_execute_builtins(t_cmd *tmp, char **et, char **ext, t_shell *t)
{
	if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
	{
		tmp->pid = 0;
		free_tab(et);
		free_tab(ext);
	}
	if (tmp->cmd && check_builtins(tmp, recup_struct_env2(NULL, 2),
			recup_struct_env2(NULL, 6), t))
	{
		tmp->pid = 0;
		if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
			return ;
		free_all(recup_struct_env2(NULL, 2), recup_struct_env2(NULL, 6), t);
		free_tab(et);
		free_tab(ext);
		exit(0);
	}
	else if (!tmp->cmd || execve(tmp->cmd, ext, et) == -1)
	{
		if (tmp->cmd)
			check_slash(tmp->cmd, 1);
		free_all(recup_struct_env2(NULL, 2), recup_struct_env2(NULL, 6), t);
		exit(127);
	}
	exit(0);
}

void	cut_last_execute(t_cmd *tmp, t_shell *tree, t_env **env, t_env **exp)
{
	char	**envtab;
	char	**exectab;

	(void)exp;
	exectab = NULL;
	envtab = NULL;
	tmp = check_in_out(tmp, tree);
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
	cut_last_execute_builtins(tmp, envtab, exectab, tree);
}

void	last_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp)
{
	t_cmd	*tmp;

	tmp = *cmd;
	if (!tmp)
		return ;
	if (tmp->cmd)
		tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
		cut_last_execute(tmp, tree, env, exp);
	else
	{
		if (tree->pipefd[0] != 0)
			close(tree->pipefd[0]);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}
