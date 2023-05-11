/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:14:52 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 15:26:59 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*checkegal(char *str, char c)
{
	int		i;
	char	*res;

	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
		{
			res[i] = 0;
			return (res);
		}
		res[i] = str[i];
		i++;
	}
	free(res);
	return (0);
}

char	*checkafteregal(char *str, char c)
{
	int		i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
		{
			str = ft_strdup(str + i + 1);
			return (str);
		}
		i++;
	}
	return (0);
}

void	add_little_env(t_env **env, int mode, t_env **exp)
{
	char	*pwd;
	char	*line;
	char	**tab;

	(void)mode;
	tab = malloc(4);
	line = ft_strdup("SHLVL=1");
	tab[0] = ft_strdup(line);
	free(line);
	pwd = getcwd(NULL, 0);
	line = ft_strjoin2("PWD=", pwd);
	tab[1] = ft_strdup(line);
	free(line);
	line = ft_strdup("OLDPWD");
	tab[2] = ft_strdup(line);
	tab[3] = 0;
	ft_export(tab, env, exp);
}

char	*update_shlvl(char *name, char *content)
{
	int	n;

	if (name && ft_strcmp(name, "SHLVL") == 0)
	{
		n = ft_atoi(content);
		if (n >= 1000)
		{
			putstr_fd_echo("minishell: warning: shell level ", 2);
			putstr_fd_echo("(", 2);
			putstr_fd_echo(content, 2);
			putstr_fd_echo(")", 2);
			putstr_fd_echo(" too hight, resetting to 1\n", 2);
			n = 1;
		}
		else if (n <= 0)
			n = 1;
		else
			n++;
		free(content);
		content = ft_itoa(n);
	}
	return (content);
}

void	get_env(t_env **env, char **envi, int mode, t_env **exp)
{
	int		i;

	i = -1;
	*env = ft_lstnew_env(NULL, NULL, NULL);
	*exp = ft_lstnew_env(NULL, NULL, NULL);
	if (!envi[0])
	{
		add_little_env(env, mode, exp);
		return ;
	}
	while (envi[++i])
		get_env_2(envi, env, exp, i);
}
