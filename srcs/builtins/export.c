/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:42:22 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 17:39:30 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_exp(char *str, t_env **exp)
{
	char	*name;

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
	char	*content;
	char	*tmp;

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
