/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/25 01:43:32 by jtaravel         ###   ########.fr       */
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

char	*check_slash(char *cmd, int mode)
{
	if (access(cmd, X_OK) == 0 && mode == 0)
	{
		/*putstr_fd_echo("minishell: ", 2);
		putstr_fd_echo(cmd, 2);
		putstr_fd_echo(": Is a directory\n", 2);
		return (NULL);*/
		return (cmd);
	}
	putstr_fd_echo("minishell: ", 2);
	putstr_fd_echo(cmd, 2);
	putstr_fd_echo(": command not found\n", 2);
	//putstr_fd_echo(": No such file or directory\n", 2);
	return (NULL);
}

char	**recover_path(char **envp)
{
	int        i;
	char    *s;
	char	**tab;
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			s = ft_strdup(envp[i] + 5);
			tab = ft_supersplit(s, ':');
			free(s);
			return (tab);
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
	char	**tab;
	
	if (is_builtins(cmd))
		return (cmd);
	if (cmd[0] == '/')
		return (check_slash(cmd, 0));
	tab = list_to_tab(env);
	newpath = recover_path(tab);
	free_tab(tab);
	while (newpath && newpath[i++])
	{
		tmp = ft_strjoin2(newpath[i], "/");
		recover = ft_strjoin(tmp, cmd);
		if (recover && access(recover, X_OK) == 0)
		{
			free_tab(newpath);
			free(cmd);
			return (recover);
		}
		free(recover);
		recover = NULL;
	}
	printf("%s : command not found\n", cmd);
	//g_rvalue = 127;
	free(cmd);
	free(recover);
	free_tab(newpath);
	return (NULL);
}

void	ft_wait(t_cmd **cmd)
{
	int	value;

	value = 0;
	t_cmd *cmd_lst = *cmd;
	waitpid(cmd_lst->pid, &value, 0);
	if (WIFSIGNALED(value))
	{
		g_rvalue = (WTERMSIG(value) + 128);
	}
	else
		g_rvalue = WEXITSTATUS(value);
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

void	del_sq_dq_arg(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		ft_suppr_dq_sq(tab[i]);
		i++;
	}
}

void	last_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	if (!tmp->cmd)
		return ;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		if (tmp->cmd)
		{
			exectab = fusioncmdarg(tmp->cmd, tmp->arg);
			envtab = list_to_tab(env);
		}
		if (tmp->name_out)
		{
			if (tmp->mode_open == 1)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC, 0644);
			else if (tmp->mode_open == 2)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644);
		}
		if (tmp->name_in)
		{
			tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
		}
		else
		{
			tmp->fd_in = tree->pipefd[0];
		}
		if (tmp->cmd)
			del_sq_dq_arg(exectab);
		if (dup2(tmp->fd_in, 0) == -1)
			perror("dup2");
		if (dup2(tmp->fd_out, 1) == -1)
			perror("dup2");
		if (tmp->cmd && check_builtins(tmp, env, exp))
		{
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(0);
		}
		else if (execve(tmp->cmd, exectab, envtab) == -1)
		{
			if (tmp->cmd)
				check_slash(tmp->cmd, 1);
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(127);
		}
		exit(0);
	}
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

void	middle_execute(t_cmd **cmd, t_env **env, t_shell *tree, int fd_temp, t_env **exp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	if (!tmp->cmd)
		return ;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		if (tmp->cmd)
		{
			exectab = fusioncmdarg(tmp->cmd, tmp->arg);
			envtab = list_to_tab(env);
		}
		if (tmp->name_out)
		{
			if (tmp->mode_open == 1)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC, 0644);
			else if (tmp->mode_open == 2)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644);
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
		if (tmp->cmd)
			del_sq_dq_arg(exectab);
		if (dup2(tmp->fd_in, 0) == -1)
			perror("dup2");
		if (dup2(tmp->fd_out, 1) == -1)
			perror("dup2");
		if (tmp->cmd && check_builtins(tmp, env, exp))
		{
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(0);
		}
		else if (execve(tmp->cmd, exectab, envtab) == -1)
		{
			if (tmp->cmd)
				check_slash(tmp->cmd, 1);
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(127);
		}
		exit(0);
	}
	else
	{
		close(tree->pipefd[1]);
		if (fd_temp != 0)
			close(fd_temp);
		if (tmp->fd_in != 0)
			close(tmp->fd_in);
		if (tmp->fd_out != 1)
			close(tmp->fd_out);
	}
}

void	first_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	if (!tmp->cmd)
		return ;
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	tmp->pid = fork();
	if (tmp->pid == 0)
	{
		if (tmp->cmd)
		{
			exectab = fusioncmdarg(tmp->cmd, tmp->arg);
			envtab = list_to_tab(env);
		}
		if (tmp->name_out)
		{
			if (tmp->mode_open == 1)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC, 0644);
			else if (tmp->mode_open == 2)
				tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644);
		}
		else
			tmp->fd_out = tree->pipefd[1];
		if (tmp->name_in)
		{
			tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
		}
		if (tmp->cmd)
			del_sq_dq_arg(exectab);
		close(tree->pipefd[0]);
		if (dup2(tmp->fd_in, 0) == -1)
			perror("dup2");
		if (dup2(tmp->fd_out, 1) == -1)
			perror("dup2");
		if (tmp->cmd && check_builtins(tmp, env, exp))
		{
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(0);
		}
		if (execve(tmp->cmd, exectab, envtab) == -1)
		{
			if (tmp->cmd)
				check_slash(tmp->cmd, 1);
			t_tree *t;
			t = recup_struct(NULL, 1);
			ft_lstcleartree(&t, del);
			free(tree);
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free_tab(envtab);
			free_tab(exectab);
			exit(127);
		}
		exit(0);
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

void	executeone(t_cmd **cmd, t_env **env, t_shell *shell, t_env **exp)
{
	t_cmd	*tmp;
	char	**envtab;
	char	**exectab;
	int	frk;

	tmp = *cmd;
	envtab = NULL;
	exectab = NULL;
	if (!tmp || !tmp->cmd)
		return ;
	ft_suppr_dq_sq(tmp->cmd);
	tmp->cmd = recup_cmd(tmp->cmd, env, 0);
	if (tmp->cmd)
	{
		exectab = fusioncmdarg(tmp->cmd, tmp->arg);
		envtab = list_to_tab(env);
	}
	if (tmp->name_out)
	{
		if (tmp->mode_open == 1)
			tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC, 0644);
		else if (tmp->mode_open == 2)
			tmp->fd_out = open(tmp->name_out, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644);
	}
	if (tmp->name_in)
		tmp->fd_in = open(tmp->name_in, O_RDONLY, 0644);
	if (tmp->cmd)
	{
		if (check_builtins(tmp, env, exp))
		{
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

void	exec(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;
	int	i;
	int	tmpfd;
	int	pipefd[2];
	char	*saveope;

	i = 0;
	saveope = NULL;
	init_heredoc(tree, env, exp, shell);
	tmp = (*tree)->next;
	pipe(shell->pipefd);
	(*tree)->next->in_exec = 1;
	signal(SIGINT, handler_fork);
	int	flag = 0;
	while (tmp)
	{
		if (tmp->cmd_right->cmd == NULL)
		{
			if (tmp->cmd_left->bracelvl)
				pars_prompt(tmp->cmd_left->cmd, *env, *exp);
			else
			{
				executeone(&tmp->cmd_left, env, shell, exp);
				ft_wait(&(tmp->cmd_left));
			}
			break ;
		}
		if (i == 0)
		{
			if (!tmp->next)
			{
				if (ft_strcmp(tmp->ope, "|") == 0)
				{
					if (tmp->cmd_left->bracelvl)
						pars_prompt(tmp->cmd_left->cmd, *env, *exp);
					first_execute(&tmp->cmd_left, env, shell, exp);
					if (tmp->cmd_left->bracelvl)
						pars_prompt(tmp->cmd_right->cmd, *env, *exp);
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_left));
					ft_wait(&(tmp->cmd_right));
				}
				else if (ft_strcmp(tmp->ope, "&&") == 0)
				{
					exec_and(&tmp->cmd_left, env, exp, shell);
					ft_wait(&(tmp->cmd_left));
					if (g_rvalue == 0)
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp);
							break ;
						}
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}
				}
				else if (ft_strcmp(tmp->ope, "||") == 0)
				{
					exec_and(&tmp->cmd_left, env, exp, shell);
					ft_wait(&(tmp->cmd_left));
					if (g_rvalue != 0)
					{
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}
				}
			}
			else if (ft_strcmp(tmp->ope, "|") == 0)
			{
				if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
				{
					first_execute(&tmp->cmd_left, env, shell, exp);
					ft_wait(&(tmp->cmd_left));
					tmpfd = shell->pipefd[0];
					pipe(shell->pipefd);
					middle_execute(&tmp->cmd_right, env, shell, tmpfd, exp);
					ft_wait(&(tmp->cmd_right));
				}
				if (tmp->next && ft_strcmp(tmp->next->ope, "&&") == 0)
				{
					first_execute(&tmp->cmd_left, env, shell, exp);
					ft_wait(&(tmp->cmd_left));
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				if (tmp->next && ft_strcmp(tmp->next->ope, "||") == 0)
				{
					first_execute(&tmp->cmd_left, env, shell, exp);
					ft_wait(&(tmp->cmd_left));
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
			else if (ft_strcmp(tmp->ope, "&&") == 0)
			{
				exec_and(&tmp->cmd_left, env, exp, shell);
				ft_wait(&(tmp->cmd_left));
				if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
				{
					first_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				else if (tmp->next && ft_strcmp(tmp->next->ope, "&&") == 0)
				{
					if (g_rvalue == 0)
					{
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}	
				}
				else if (tmp->next && ft_strcmp(tmp->next->ope, "||") == 0)
				{
					if (g_rvalue == 0)
					{
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}	
				}
			}
			else if (ft_strcmp(tmp->ope, "||") == 0)
			{
				exec_and(&tmp->cmd_left, env, exp, shell);
				ft_wait(&(tmp->cmd_left));
				if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0 && g_rvalue != 0)
				{
					first_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				else if (tmp->next && ft_strcmp(tmp->next->ope, "||") == 0)
				{
					if (g_rvalue != 0)
					{
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}	
				}
				else if (tmp->next && ft_strcmp(tmp->next->ope, "&&") == 0)
				{
					if (g_rvalue != 0)
					{
						exec_and(&tmp->cmd_right, env, exp, shell);
						ft_wait(&(tmp->cmd_right));
					}	
				}
			}
		}
		else
		{
			if (tmp->next && ft_strcmp(saveope, "|") == 0 && ft_strcmp(tmp->ope, "|") == 0
				&& ft_strcmp(tmp->next->ope, "|") == 0)
			{
	//			printf("CMD 1 = %s\n", tmp->cmd_right->cmd);
				tmpfd = shell->pipefd[0];
				pipe(shell->pipefd);
				middle_execute(&tmp->cmd_right, env, shell, tmpfd, exp);
				ft_wait(&(tmp->cmd_right));
			}
			else if (!tmp->next && ft_strcmp(saveope, "|") == 0 && ft_strcmp(tmp->ope, "|") == 0)
			{
	//			printf("CMD 2 = %s\n", tmp->cmd_right->cmd);
				last_execute(&tmp->cmd_right, env, shell, exp);
				ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(saveope, "|") == 0 && ft_strcmp(tmp->ope, "&&") == 0)
			{
				if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
				{
	//				printf("CMD 5 = %s\n", tmp->cmd_right->cmd);
					pipe(shell->pipefd);
					first_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				else
				{
	//				printf("CMD 3 = %s\n", tmp->cmd_right->cmd);
					exec_and(&tmp->cmd_right, env, exp, shell);
					//last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
			else if (tmp->next && ft_strcmp(tmp->ope, "&&") == 0 && ft_strcmp(tmp->next->ope, "|") == 0)
			{
	//			printf("CMD 4 = %s\n", tmp->cmd_right->cmd);
				first_execute(&tmp->cmd_right, env, shell, exp);
				ft_wait(&(tmp->cmd_right));
			}
			else if (!tmp->next && ft_strcmp(saveope, "&&") == 0 && ft_strcmp(tmp->ope, "|") == 0)
					//&& ft_strcmp(tmp->next->ope, "|") != 0)
			{
				//pipe(shell->pipefd);
				last_execute(&tmp->cmd_right, env, shell, exp);
				ft_wait(&(tmp->cmd_left));
			}
			else if (ft_strcmp(saveope, "&&") == 0 && ft_strcmp(tmp->ope, "&&") == 0 && g_rvalue == 0)
			{
				if (tmp->cmd_right->bracelvl)
					pars_prompt(tmp->cmd_right->cmd, *env, *exp);
				exec_and(&tmp->cmd_right, env, exp, shell);
				ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(saveope, "&&") == 0 && ft_strcmp(tmp->ope, "||") == 0 && g_rvalue != 0)
			{
				exec_and(&tmp->cmd_right, env, exp, shell);
				ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(saveope, "|") == 0 && ft_strcmp(tmp->ope, "||") == 0 && g_rvalue != 0)
			{
				exec_and(&tmp->cmd_right, env, exp, shell);
				ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(saveope, "||") == 0 && ft_strcmp(tmp->ope, "&&") == 0 && g_rvalue == 0)
			{
				exec_and(&tmp->cmd_right, env, exp, shell);
				ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(saveope, "||") == 0 && ft_strcmp(tmp->ope, "||") == 0 && g_rvalue != 0)
			{
				exec_and(&tmp->cmd_right, env, exp, shell);
				ft_wait(&(tmp->cmd_right));
			}
		}
		/*if (i == 0)
		{
			if (tmp->ope && ft_strcmp(tmp->ope, "|") == 0)
			{
				first_execute(&tmp->cmd_left, env, shell, exp);
				ft_wait(&(tmp->cmd_left));
				if (!tmp->next || ft_strcmp(tmp->next->ope, "|") != 0)
				{
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				else
				{
					tmpfd = tmp->pipefd[0];
					pipe(tmp->pipefd);
					middle_execute(&tmp->cmd_right, env, shell, tmpfd, exp);
					ft_wait(&(tmp->cmd_right));
				}

			}
			else if (tmp->ope && ft_strcmp(tmp->ope, "&&") == 0)
			{
				exec_and(&tmp->cmd_left, env, exp);
				ft_wait(&(tmp->cmd_left));
				if (!tmp->next && g_rvalue == 0)
				{
					exec_and(&tmp->cmd_right, env, exp);
					ft_wait(&(tmp->cmd_right));
					return ;
				}
				else if (g_rvalue == 0 && ft_strcmp(tmp->next->ope, "|") != 0)
				{
					exec_and(&tmp->cmd_right, env, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
		}
		else
		{
			if (tmp->ope && ft_strcmp(tmp->ope, "|") == 0
				&& tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
			{
				if (ft_strcmp(saveope, "&&") == 0 && g_rvalue == 0)
				{
					flag = 1;
					pipe(shell->pipefd);
					first_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
			else if (tmp->ope && ft_strcmp(tmp->ope, "|") == 0)
			{
				if (!tmp->next && ft_strcmp(tmp->ope, "|") == 0 && flag)
				{
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_right));
				}
				else
				{
					if (tmp->next && ft_strcmp(tmp->next->ope, "&&") == 0)
					{
						last_execute(&tmp->cmd_right, env, shell, exp);
						ft_wait(&(tmp->cmd_right));
					}
					else if (flag)
					{
						tmpfd = tmp->pipefd[0];
						pipe(tmp->pipefd);
						middle_execute(&tmp->cmd_right, env, shell, tmpfd, exp);
						ft_wait(&(tmp->cmd_right));
					}
				}
			}
			else if (tmp->ope && ft_strcmp(tmp->ope, "&&") == 0 && !tmp->next)
			{
				if (g_rvalue == 0)
				{
					exec_and(&tmp->cmd_right, env, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
			else if (tmp->ope && ft_strcmp(tmp->ope, "&&") == 0 && tmp->next
				&& ft_strcmp(tmp->next->ope, "|") != 0)
			{
				if (g_rvalue == 0)
				{
					exec_and(&tmp->cmd_right, env, exp);
					ft_wait(&(tmp->cmd_right));
				}
			}
			else if (tmp->ope && ft_strcmp(tmp->ope, "&&") == 0 && tmp->next
				&& ft_strcmp(tmp->next->ope, "|") == 0)
			{
				if (g_rvalue == 0)
				{
					flag = 1;

					//first_execute(&tmp->cmd_right, env, tmp->pipefd, exp);
					//middle_execute(&tmp->cmd_right, env, tmp->pipefd, tmpfd, exp);
					//exec_and(&tmp->cmd_right, env, exp);
					//ft_wait(&(tmp->cmd_right));
				}
			}
		}*/
		i++;
		if (saveope)
		{
			free(saveope);
			saveope = NULL;
		}
		saveope = ft_strdup(tmp->ope);
		tmp = tmp->next;
	}
	if (saveope)
	{
		free(saveope);
		saveope = NULL;
	}
//	signal(SIGINT, handler);
	(*tree)->next->in_exec = 0;
	/*tmp = (*tree)->next;
	int j = 0;
	while (tmp)
	{
		if (j == 0)
		{
			ft_wait(&(tmp->cmd_left));
			if (tmp->cmd_right->cmd)
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
	}*/
	
}
