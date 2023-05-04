/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:14:52 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 17:27:53 by jtaravel         ###   ########.fr       */
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
	return (0);
}

char	*checkafteregal(char *str, char c)
{
	int		i;
	char	**tab;

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

void	get_env(t_env **env, char **envi, int mode, t_env **exp)
{
	t_env	*tmp;
	char	*name;
	char	*content;
	char	*str;
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
	{
		name = checkegal(envi[i], '=');
		content = checkafteregal(envi[i], '=');
		ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(envi[i]),
				ft_strdup(name), ft_strdup(content)));
		ft_lstadd_back_env(exp, ft_lstnew_env(ft_strdup(envi[i]),
				ft_strdup(name), ft_strdup(content)));
		free(name);
		free(content);
	}
}
