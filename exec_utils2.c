/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:46:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 11:18:16 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cut_recup_cmd(char **newpath, char *cmd, char *tmp, int i)
{
	char	*recover;

	recover = NULL;
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
	if (recover)
		free(recover);
	free_tab(newpath);
	return (NULL);
}

char	*recup_cmd(char *cmd, t_env **env, int i)
{
	char	**newpath;
	char	*tmp;
	char	*recover;
	char	**tab;

	tmp = NULL;
	if (is_builtins(cmd))
		return (cmd);
	if (cmd[0] == '/')
		return (check_slash(cmd, 0));
	if (cmd[0] == '.' && check_dot(cmd))
		return (cmd);
	tab = list_to_tab(env);
	newpath = recover_path(tab);
	free_tab(tab);
	recover = NULL;
	return (cut_recup_cmd(newpath, cmd, tmp, i));
}

void	cut_middle_error(char *str, t_shell *tree)
{
	putstr_fd_echo("minishell: ", 2);
	putstr_fd_echo(str, 2);
	putstr_fd_echo(": No such file or directory\n", 2);
	free_all(recup_struct_env2(NULL, 2),
		recup_struct_env2(NULL, 6), tree);
	exit(1);
}
