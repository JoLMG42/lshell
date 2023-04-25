/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:36:52 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/25 01:44:37 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*recup_content_env(char *find, t_env **env)	
{
	t_env	*tmp;

	tmp = (*env)->next;
	while (tmp->next)
	{
		if (ft_strcmp(find, tmp->name) == 0)
			return (ft_strdup(tmp->content));
		tmp = tmp->next;
	}
	return (NULL);
}

void	create_oldpwd(t_env **env)
{
	char	*str;
	char	*line;
	char	*name;

	str = recup_content_env("PWD", env);
	if (!str)
		str = getcwd(NULL, 0);
	name = ft_strdup("PWD=");
	line = ft_strjoin(name, str);
	ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(line), ft_strdup(name), ft_strdup(str)));
	ft_env(env);
}

void	update_pwd_oldpwd(t_env **env, char *newpath, int mode)
{
	t_env	*tmp;
	char	*str;

	if (recup_content_env("OLDPWD", env) == NULL)
		create_oldpwd(env);
	tmp = (*env)->next;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->name, "OLDPWD") == 0)
		{
			free(tmp->content);
			tmp->content = recup_content_env("PWD", env);
		}
		tmp = tmp->next;
	}
	tmp = (*env)->next;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->name, "PWD") == 0)
		{
			if (mode == 1)
			{
				free(tmp->content);
				tmp->content = ft_strdup(newpath);
			}
			else if (mode == 2)
			{
				
				free(tmp->content);
				str = getcwd(NULL, 0);
				tmp->content = ft_strdup(str);
				free(str);

			}
		}
		tmp = tmp->next;
	}
}

void	cd_no_arg(t_env **env)
{
	char	*str;
	
	str = recup_content_env("HOME", env);
	if (!str)
		putstr_fd_echo("minishell: cd: HOME not set\n", 2);
	if (chdir(str) == -1)
	{
		free(str);
		return ;
	}
	update_pwd_oldpwd(env, str, 1);
	free(str);
}

void	cd_tild(t_env **env)
{
	(void)env;
	putstr_fd_echo("cd ~ non fonctionnel\n", 2);
}

void	cd_moins(t_env **env)
{
	char	*str;

	str = recup_content_env("OLDPWD", env);
	if (!str)
	{
		putstr_fd_echo("minishell: cd: OLDPWD not set\n", 2);
		return ;
	}
	if (chdir(str) == -1)
	{
		putstr_fd_echo("minishell: cd: ", 2);
		putstr_fd_echo(str, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		free(str);
		return ;
	}
	putstr_fd_echo(str, 1);
	putstr_fd_echo("\n", 1);
	free(str);
	return ;
}

void	cd_arg(char *str, t_env **env)
{
	if (ft_strcmp(str, "~") == 0)
	{
		cd_tild(env);
		return ;
	}
	else if (ft_strcmp(str, "-") == 0)
	{
		cd_moins(env);
		return ;
	}
	else if (chdir(str) == -1)
	{
		putstr_fd_echo("minishell: cd: ", 2);
		putstr_fd_echo(str, 2);
		putstr_fd_echo(": No such file or directory\n", 2);
		return ;
	}
	update_pwd_oldpwd(env, str, 2);
}


void	ft_cd(char **tab, t_env **env)
{
	if (tab_len(tab) == 0)
		cd_no_arg(env);
	else if (tab_len(tab) > 1)
	{
		putstr_fd_echo("minishell: cd: too many arguments\n", 2);
	}
	else
		cd_arg(tab[0], env);
}
