/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:36:52 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 20:21:09 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*recup_content_env(char *find, t_env **env)	
{
	t_env	*tmp;

	tmp = (*env)->next;
	while (tmp)
	{
		if (ft_strcmp(find, tmp->name) == 0)
			return (ft_strdup(tmp->content));
		tmp = tmp->next;
	}
	return (NULL);
}

void	create_oldpwd(t_env **env, int mode)
{
	char	*str;
	char	*line;
	char	*name;

	if (mode == 0)
	{
		str = getcwd(NULL, 0);
		name = ft_strdup("PWD");
	}
	else if (mode == 1)
	{
		str = getcwd(NULL, 0);
		name = ft_strdup("OLDPWD");
	}
	line = ft_strjoin(name, "=");
	line = ft_strjoin(line, str);

	ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(line), ft_strdup(name), ft_strdup(str)));
}

void	update_pwd_oldpwd(t_env **env, char *newpath, int mode, t_env **exp)
{
	t_env	*tmp;
	char	*str;
	int	flag;
	char	**tab;
	char	*line;

	tab = malloc(sizeof(char *) * 3);
	flag = 0;
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
	str = recup_content_env("PWD", env);
	if (!str)
		str = getcwd(NULL, 0);
	line = ft_strjoin(line, str);
	free(str);
	tab[1] = ft_strdup(line);
	free(line);
	tab[2] = 0;
	ft_export(tab, env, exp);
	free_tab(tab);
}

void	cd_no_arg(t_env **env, t_env **exp)
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
	update_pwd_oldpwd(env, str, 1, exp);
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

void	check_pwd_oldpwd(t_env **env)
{
	if (recup_content_env("PWD", env) == NULL)
	{
		printf("111\n");
		create_oldpwd(env, 0);
	}
	if (recup_content_env("OLDPWD", env) == NULL)
	{
		printf("222\n");
		create_oldpwd(env, 1);
	}
}

void	cd_arg(char *str, t_env **env, t_env **exp)
{
	//check_pwd_oldpwd(env);
	if (ft_strcmp(str, "~") == 0)
	{
		cd_tild(env);
		return ;
	}
	else if (ft_strcmp(str, "-") == 0)
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
	}
	else
		cd_arg(tab[0], env, exp);
}
