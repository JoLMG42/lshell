/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:41:51 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/09 23:59:43 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**fusioncmdarg(char *cmd, char **arg)
{
	char	**res;
	int		i;
	int		j;

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
		return (cmd);
	}
	putstr_fd_echo("minishell: ", 2);
	putstr_fd_echo(cmd, 2);
	putstr_fd_echo(": command not found\n", 2);
	free(cmd);
	return (NULL);
}

char	**recover_path(char **envp)
{
	int		i;
	char	*s;
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

int	check_dot(char *str)
{
	if (access(str, X_OK) == 0)
		return (1);
	return (0);
}
