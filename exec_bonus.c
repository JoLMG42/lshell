/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 17:04:44 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 01:45:11 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_cmd **cmd, t_env **env, t_env **exp, t_shell *shell)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	if (!tmp || !tmp->cmd)
		return ;
	ft_suppr_dq_sq(tmp->cmd);
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->fd_in = 0;
	tmp->fd_out = 1;
	if (tmp->cmd)
	{
		exectab = fusioncmdarg(tmp->cmd, tmp->arg);
		envtab = list_to_tab(env);
	}
	if (tmp->name_out)
		tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY, 0644);
	if (tmp->name_in)
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	if (tmp->cmd)
	{
		if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
		{
			free_tab(envtab);
			free_tab(exectab);
		}

		if (check_builtins(tmp, env, exp, shell))
		{
			if (tmp->cmd && ft_strcmp(tmp->cmd, "exit") == 0)
				return ;
			free_tab(exectab);
			free_tab(envtab);
			return ;
		}
	}
	if (tmp->cmd)
		del_sq_dq_arg(exectab);
	g_rvalue = 0;
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (execve(tmp->cmd, exectab, envtab) == -1)
		{
			if (tmp->cmd)
				check_slash(tmp->cmd, 1);
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(shell);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(127);
		}
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

