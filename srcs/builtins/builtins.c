/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:54:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 18:30:25 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rejoinstr(char **tab)
{
	int		i;
	int		j;
	int		c;
	char	*res;

	i = -1;
	c = 0;
	while (tab[++i])
	{
		j = -1;
		while (tab[i][++j])
			c++;
		c++;
	}
	res = NULL;
	i = -1;
	while (tab[++i])
	{
		res = ft_strjoin(res, tab[i]);
		if (tab[i + 1])
			res = ft_strjoin(res, " ");
	}
	return (res);
}

int	is_builtins(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

int	check_builtins_2(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell)
{
	(void)shell;
	if (ft_strcmp(cmd->cmd, "pwd") == 0)
	{
		ft_pwd(cmd);
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "cd") == 0)
	{
		ft_cd(cmd->arg, env, exp);
		if (g_rvalue != 1)
			g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "export") == 0)
	{
		ft_export(cmd->arg, env, exp);
		if (g_rvalue != 1)
			g_rvalue = 0;
		return (1);
	}
	return (0);
}

int	check_builtins(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
	{
		ft_echo(cmd, cmd->arg, env);
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "env") == 0)
	{
		ft_env(cmd, env);
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "unset") == 0)
	{
		ft_unset(cmd->arg, env, exp);
		if (g_rvalue != 1)
			g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "exit") == 0)
	{
		ft_exit(cmd, env, exp, shell);
		return (1);
	}
	return (check_builtins_2(cmd, env, exp, shell));
}
