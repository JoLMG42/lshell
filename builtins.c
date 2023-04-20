/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:54:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/20 16:05:09 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*rejoinstr(char **tab)
{
	int	i;
	int	j;
	int	c;
	char	*res;

	i = 0;
	c = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			c++;
			j++;
		}
		c++;
		i++;
	}
	res = malloc(sizeof(char) * (c + 1));
	i = 0;
	while (tab[i])
	{
		res = ft_strjoin(res, tab[i]);
		if (tab[i + 1])
			res = ft_strjoin(res, " ");
		i++;
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

int	check_builtins(t_cmd *cmd, t_env **env, t_env **exp)
{
	char	*str;

	if (ft_strcmp(cmd->cmd, "echo") == 0)
	{
		ft_echo(cmd->arg, env);
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "env") == 0)
	{
		ft_env(env);
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "unset") == 0)
	{
		ft_unset(cmd->arg, env);
		if (g_rvalue != 1)
			g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "cd") == 0)
	{
		ft_cd(cmd->arg, env);
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
	if (ft_strcmp(cmd->cmd, "pwd") == 0)
	{
		ft_pwd();
		g_rvalue = 0;
		return (1);
	}
	if (ft_strcmp(cmd->cmd, "exit") == 0)
	{
		return (1);
	}
	return (0);
}
