/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 16:08:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 18:24:48 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_no_arg(t_env **env, t_env **exp)
{
	char	*str;

	str = recup_content_env("HOME", env);
	if (!str)
	{
		putstr_fd_echo("minishell: cd: HOME not set\n", 2);
		g_rvalue = 1;
	}
	if (chdir(str) == -1)
	{
		free(str);
		return ;
	}
	update_pwd_oldpwd(env, str, 1, exp);
	free(str);
}

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

	name = NULL;
	str = NULL;
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
	ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(line), ft_strdup(name),
			ft_strdup(str)));
}
