/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:42:22 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 20:24:53 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_egal(char *str);

void	export_no_arg(t_env **exp)
{
	t_env	*tmp;

	tmp = (*exp);
	while (tmp)
	{
		if (!check_egal(tmp->line))
			printf("export %s\n", tmp->name);
		else
			printf("export %s=\"%s\"\n", tmp->name, tmp->content);
		tmp = tmp->next;
	}
}

int	check_egal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	len_egal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i + 1);
}

char	*before_egal(char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen(str)));
	if (!res)
		return (NULL);
	while (str && str[i] && str[i] != '=')
	{
		res[j] = str[i];
		j++;
		i++;
	}
	res[j] = 0;
	return (res);
}

t_env	*var_in_exp(char *str, t_env **exp)
{
	t_env	*tmp;

	tmp = (*exp)->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, str) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	export_exp(char *str, t_env **exp)
{
	char	*name;
	char	*tmp;

	if (var_in_exp(str, exp) != NULL)
		return ;
	name = ft_strdup(str);
	if (!check_name(name))
	{
		putstr_fd_echo("minishell: export: '", 2);
		putstr_fd_echo(name, 2);
		putstr_fd_echo("': not a valid identifier\n", 2);
		g_rvalue = 1;
		free(name);
		return ;
	}
	ft_lstadd_back_env(exp, ft_lstnew_env(ft_strdup(name),
			ft_strdup(name), NULL));
	free(name);
}

int	check_plus(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
			return (1);
		i++;
	}
	return (0);
}

void	cut_export_join_error(char *tmp)
{
	putstr_fd_echo("minishell: export: '", 2);
	putstr_fd_echo(tmp, 2);
	putstr_fd_echo("': not a valid identifier\n", 2);
	g_rvalue = 1;
	free(tmp);
	return ;
}

void	cut_export_join_unset(t_env **env, t_env **exp, t_env *recup)
{
	char	**tab;

	tab = malloc(sizeof(char *) * 3);
	tab[0] = ft_strdup(recup->name);
	tab[1] = 0;
	ft_unset(tab, env, exp);
	free_tab(tab);
}

void	cut_join_content(char *name, char *content, t_env **env, t_env **exp)
{
	t_env	*recup;
	char	*join;
	char	*tmp;

	tmp = ft_strdup(name);
	tmp[ft_strlen(tmp) - 1] = 0;
	recup = var_in_exp(tmp, exp);
	join = NULL;
	join = ft_strjoin(join, tmp);
	join = ft_strjoin(join, "=");
	if (recup && recup->content)
		join = ft_strjoin(join, recup->content);
	join = ft_strjoin(join, content);
	if (recup)
		cut_export_join_unset(env, exp, recup);
	ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(join),
			ft_strdup(tmp), ft_strdup(content)));
	if (recup)
		ft_lstadd_back_env(exp, ft_lstnew_env(ft_strdup(join),
				ft_strdup(tmp), ft_strdup(join + ft_strlen(name))));
	else
		ft_lstadd_back_env(exp, ft_lstnew_env(ft_strdup(join),
				ft_strdup(tmp), ft_strdup(content)));
	free(tmp);
	free(join);
}

void	join_content(char *content, char *name, t_env **env, t_env **exp)
{
	char	*tmp;

	tmp = ft_strdup(name);
	if (!check_name(tmp))
	{
		cut_export_join_error(tmp);
		return ;
	}
	free(tmp);
	cut_join_content(name, content, env, exp);
}

void	cut_export_both(t_env **env, t_env **exp, char *content, char *str)
{
	char	**tab;
	char	*tmp;
	char	*name;

	tmp = before_egal(str);
	tab = malloc(sizeof(char *) * 2);
	tab[0] = ft_strdup(tmp);
	tab[1] = 0;
	ft_unset(tab, env, exp);
	free_tab(tab);
	name = ft_strdup(tmp);
	free(tmp);
	ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(str),
			ft_strdup(name), ft_strdup(content)));
	ft_lstadd_back_env(exp, ft_lstnew_env(ft_strdup(str),
			ft_strdup(name), ft_strdup(content)));
	free(name);
}

void	export_both(char *str, t_env **env, t_env **exp)
{
	char	*name;
	char	*content;
	char	*tmp;
	t_env	*recup;

	tmp = before_egal(str);
	if (!check_name(tmp))
	{
		cut_export_join_error(tmp);
		return ;
	}
	content = ft_strdup(str + len_egal(str));
	if (check_plus(tmp))
	{
		join_content(content, tmp, env, exp);
		free(tmp);
		free(content);
		return ;
	}
	cut_export_both(env, exp, content, str);
	free(tmp);
	free(content);
}

int	check_name(char *str)
{
	int	i;

	if (str[0] >= 48 && str[0] <= 57)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '_' || (str[i] >= 48 && str[i] <= 57)
			|| (str[i] >= 65 && str[i] <= 90)
			|| (str[i] >= 97 && str[i] <= 122)
			|| (str[i] == '+'))
			i++;
		else
			return (0);
	}
	return (1);
}

void	export_arg(char **tab, t_env **exp, t_env **env)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		ft_suppr_dq_sq(tab[i]);
		if (!check_egal(tab[i]))
		{
			export_exp(tab[i], exp);
		}
		else
			export_both(tab[i], env, exp);
		i++;
	}
}

void	ft_export(char **tab, t_env **env, t_env **exp)
{
	t_env	*tmpexp;
	t_env	*tmpenv;

	tmpenv = (*env)->next;
	tmpexp = (*exp)->next;
	if (tmpenv == NULL)
	{
		tmpenv = (*env);
		tmpexp = (*exp);
	}
	if (tab_len(tab) < 1)
	{
		export_no_arg(&tmpexp);
		return ;
	}
	else
	{
		export_arg(tab, &tmpexp, &tmpenv);
	}
}
