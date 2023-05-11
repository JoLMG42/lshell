/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:36:52 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 12:47:38 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd_oldpwd2(t_env **env, char *line, char **tab, t_env **exp)
{
	tab[1] = ft_strdup(line);
	free(line);
	tab[2] = 0;
	ft_export(tab, env, exp);
	free_tab(tab);
}

void	update_pwd_oldpwd(t_env **env, char *newpath, int mode, t_env **exp)
{
	char	*str;
	char	**tab;
	char	*line;

	tab = malloc(sizeof(char *) * 3);
	line = ft_strdup("PWD=");
	if (mode == 1)
		line = ft_strjoin(line, newpath);
	else
	{
		str = getcwd(NULL, 0);
		line = ft_strjoin(line, str);
		free(str);
	}
	tab[0] = ft_strdup(line);
	free(line);
	line = ft_strdup("OLDPWD=");
	str = ft_strdup(recup_content_env("PWD", env));
	if (!str)
		str = getcwd(NULL, 0);
	line = ft_strjoin(line, str);
	free(str);
	update_pwd_oldpwd2(env, line, tab, exp);
}

void	cd_moins(t_env **env)
{
	char	*str;

	str = recup_content_env("OLDPWD", env);
	if (!str)
	{
		putstr_fd_echo("minishell: cd: OLDPWD not set\n", 2);
		g_rvalue = 1;
		return ;
	}
	if (chdir(str) == -1)
	{
		putstr_fd_echo("minishell: cd: ", 2);
		putstr_fd_echo(str, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		g_rvalue = 1;
		free(str);
		return ;
	}
	putstr_fd_echo(str, 1);
	putstr_fd_echo("\n", 1);
	return ;
}

void	cd_arg(char *str, t_env **env, t_env **exp)
{
	if (ft_strcmp(str, "-") == 0)
	{
		cd_moins(env);
		update_pwd_oldpwd(env, str, 2, exp);
		return ;
	}
	else if (chdir(str) == -1)
	{
		putstr_fd_echo("minishell: cd: ", 2);
		putstr_fd_echo(str, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		g_rvalue = 1;
		return ;
	}
	update_pwd_oldpwd(env, str, 2, exp);
}

void	ft_cd(char **tab, t_env **env, t_env **exp)
{
	if (tab_len(tab) == 0)
		cd_no_arg(env, exp);
	else if (tab_len(tab) > 1)
	{
		putstr_fd_echo("minishell: cd: too many arguments\n", 2);
		g_rvalue = 1;
	}
	else
		cd_arg(tab[0], env, exp);
}
