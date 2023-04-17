/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/17 14:10:25 by jtaravel         ###   ########.fr       */
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
		return (cmd);
	}
	printf("%s: %s\n", "access denied", cmd);
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
	free_tab(newpath);
	return (NULL);
}

void	execute(t_cmd **cmd, t_env **env)
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
	frk = fork();
	if (frk == 0)
	{
		if (execve(tmp->cmd, exectab, envtab) == -1)
			printf("lol\n");
	}
	else
		wait (NULL);
}

void	exec(t_tree **tree, t_env **env)
{
	t_tree	*tmp;
	int	i;

	i = 0;
	tmp = (*tree)->next;
	while (tmp)
	{
		if (i == 0)
		{
			execute(&tmp->cmd_left, env);
		}
		tmp = tmp->next;
	}
}
