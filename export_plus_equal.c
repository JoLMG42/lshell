/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_plus_equal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:34:14 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 17:35:19 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
