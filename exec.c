/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/19 23:46:29 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**fusioncmdarg(char *cmd, char **arg)
{
	char	**res;
	int	i;
	int	j;

	res = malloc(sizeof(char *) * (tab_len(arg) + 2));
	res[0] = ft_strdup(cmd);
	i = 0;
	j = 1;
	while (arg[i])
	{
		res[j] = ft_strdup(arg[i]);
		i++;
		j++;
	}
	res[j] = 0;
	return (res);
}

char	*check_slash(char *cmd)
{
	if (access(cmd, X_OK) == 0)
	{
		putstr_fd_echo("minishell: ", 2);
		putstr_fd_echo(cmd, 2);
		putstr_fd_echo(": Is a directory\n", 2);
		return (NULL);
	}
	putstr_fd_echo("minishell: ", 2);
	putstr_fd_echo(cmd, 2);
	putstr_fd_echo(": No such file or directory\n", 2);
	return (NULL);
}

char	**recover_path(char **envp)
{
	int        i;
	char    *s;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			s = envp[i] + 5;
			return (ft_supersplit(s, ':'));
		}
		i++;
	}
	return (NULL);
}

char	*recup_cmd(char *cmd, t_env **env, int i)
{
	t_env *tmpenv = (*env)->next;
	char	**newpath;
	char	*tmp;
	char	*recover;
	
	if (is_builtins(cmd))
		return (cmd);
	if (cmd[0] == '/')
		return (check_slash(cmd));
	newpath = recover_path(list_to_tab(env));
	recover = ft_strdup(cmd);
	while (newpath && newpath[i++])
	{
		tmp = ft_strjoin(newpath[i], "/");
		recover = ft_strjoin(tmp, cmd);
		//free(tmp);
		if (recover && access(recover, X_OK) == 0)
		{
		//	free_tab(newpath);
			free(cmd);
			return (recover);
		}
		free(recover);
		recover = NULL;
	}
	printf("%s : command not found\n", cmd);
	free(cmd);
	//free_tab(newpath);
	return (NULL);
}

void	ft_wait(t_cmd **cmd)
{
	int value = 0;
	t_cmd *cmd_lst = *cmd;
	while (cmd_lst)
	{
		if (cmd_lst->next == NULL)
		{
			waitpid(cmd_lst->pid, &g_rvalue, 0);
			if (WIFSIGNALED(g_rvalue))
			{
				g_rvalue = (WTERMSIG(g_rvalue) + 128);
			}
			else
				g_rvalue = WEXITSTATUS(g_rvalue);
			break ;
		}
		waitpid(cmd_lst->pid, &g_rvalue, 0);
		if (WIFSIGNALED(g_rvalue) && WIFSIGNALED(g_rvalue) != 1)
			value = WTERMSIG(g_rvalue) + 128;
		else
			value = WEXITSTATUS(g_rvalue);
		cmd_lst = cmd_lst->next;
	}
}

void	handler_fork(int sig)
{
	if (sig == 2)
	{
		g_rvalue = 130;
		printf("\n");
		return ;
	}
}

void	last_execute(t_cmd **cmd, t_env **env, int pipefd[2])
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	if (!tmp->cmd)
		return ;
	exectab = fusioncmdarg(tmp->cmd, tmp->arg);
	envtab = list_to_tab(env);
	if (tmp->name_out)
	{
		tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY, 0644);
	}
	if (tmp->name_in)
	{
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	}
	else
		tmp->fd_in = pipefd[0];
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (execve(tmp->cmd, exectab, envtab) == -1)
			printf("lol\n");
	}
	else
	{
		if (pipefd[0] != 0)
			close(pipefd[0]);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}

void	middle_execute(t_cmd **cmd, t_env **env, int pipefd[2], int fd_temp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	if (!tmp->cmd)
		return ;
	exectab = fusioncmdarg(tmp->cmd, tmp->arg);
	envtab = list_to_tab(env);
	if (tmp->name_out)
	{
		tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY, 0644);
	}
	else
	{
		tmp->fd_out = pipefd[1];
		close(pipefd[0]);
	}
	if (tmp->name_in)
	{
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	}
	else
		tmp->fd_in = fd_temp;
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (execve(tmp->cmd, exectab, envtab) == -1)
			printf("lol\n");
	}
	else
	{
		close(pipefd[1]);
		close(fd_temp);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}

void	first_execute(t_cmd **cmd, t_env **env, int pipefd[2])
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	if (!tmp->cmd)
		return ;
	exectab = fusioncmdarg(tmp->cmd, tmp->arg);
	envtab = list_to_tab(env);
	close(pipefd[0]);
	if (tmp->name_out)
	{
		tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY, 0644);
	}
	else
		tmp->fd_out = pipefd[1];
	if (tmp->name_in)
	{
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	}
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (execve(tmp->cmd, exectab, envtab) == -1)
			printf("lol\n");
	}
	else
	{
		close(pipefd[1]);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}

void	executeone(t_cmd **cmd, t_env **env, int pipefd[2], t_env **exp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	if (!tmp || !tmp->cmd)
		return ;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	if (!tmp->cmd)
		return ;
	tmp->fd_in = 0;
	tmp->fd_out = 1;
	exectab = fusioncmdarg(tmp->cmd, tmp->arg);
	envtab = list_to_tab(env);
	if (tmp->name_out)
		tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY, 0644);
	if (tmp->name_in)
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	if (check_builtins(tmp, env, exp))
		return ;
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		dup2(tmp->fd_in, 0);
		dup2(tmp->fd_out, 1);
		if (execve(tmp->cmd, exectab, envtab) == -1)
			printf("lol\n");
		exit(127);
	}
	else
	{
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}

void	exec(t_tree **tree, t_env **env, t_env **exp)
{
	t_tree	*tmp;
	int	i;
	int	pipefd[2];
	int	tmpfd;

	i = 0;
	tmp = (*tree)->next;
	pipe(tmp->pipefd);
	(*tree)->next->in_exec = 1;
	signal(SIGINT, handler_fork);
	while (tmp)
	{
		if (tmp->cmd_right->cmd == NULL)
		{
			if (tmp->cmd_left->is_hd)
				heredoc(&tmp->cmd_left);
			executeone(&tmp->cmd_left, env, tmp->pipefd, exp);
		}
		if (i == 0)
		{
			if (tmp->ope && ft_strcmp(tmp->ope, "|") == 0)
			{
				//tmpfd = pipefd[1];
				first_execute(&tmp->cmd_left, env, tmp->pipefd);
				//pipefd[0] = tmpfd;
				if (!tmp->next)
				{
					last_execute(&tmp->cmd_right, env, tmp->pipefd);
				}
				else
				{
					tmpfd = tmp->pipefd[0];
					pipe(tmp->pipefd);
					middle_execute(&tmp->cmd_right, env, tmp->pipefd, tmpfd);
				}

			}
		}
		else
		{
			if (!tmp->next)
				last_execute(&tmp->cmd_right, env, tmp->pipefd);
			else
			{
				tmpfd = tmp->pipefd[0];
				pipe(tmp->pipefd);
				middle_execute(&tmp->cmd_right, env, tmp->pipefd, tmpfd);
			}
		}
		i++;
		tmp = tmp->next;
	}
	//signal(SIGINT, handler);
	(*tree)->next->in_exec = 0;
	tmp = (*tree)->next;
	int j = 0;
	while (tmp)
	{
		if (j == 0)
		{
			ft_wait(&(tmp->cmd_left));
			if (tmp->cmd_right)
			{
				ft_wait(&(tmp->cmd_right));
			}
		}
		else
		{
			ft_wait(&(tmp->cmd_right));
		}
		tmp = tmp->next;
		j++;
	}
	
}
