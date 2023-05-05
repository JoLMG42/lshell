/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:48:20 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/05 14:47:36 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cut_sds(char *str, t_s *s, int i, int c)
{
	int	j;

	j = i - 1;
	if (c == 1)
	{
		if (s->sq_opened)
			s->sq_opened = 0;
		else
			s->sq_opened = 1;
		while (str[++j])
			str[j] = str[j + 1];
	}
	if (c == 2)
	{
		if (s->dq_opened)
			s->dq_opened = 0;
		else
			s->dq_opened = 1;
		while (str[++j])
			str[j] = str[j + 1];
	}
	str[j] = 0;
	return (str);
}

char	*cut_reparse_dol(char *res, t_env **env, char *str)
{
	t_env	*tmp;

	tmp = var_in_exp(res, env);
	free(res);
	res = NULL;
	if (!tmp)
		return (str);
	res = ft_strjoin(res, "'");
	res = ft_strjoin(res, tmp->content);
	res = ft_strjoin(res, "'");
	free(str);
	return (res);
}

char	*reparse_dol(char *str, t_env **env)
{
	char	*res;
	int		i;
	char	*truc;

	i = 0;
	if (str[0] != '\'')
		return (str);
	res = ft_strdup(str + 1);
	res[ft_strlen(res) - 1] = 0;
	if (res[0] == '$')
	{
		truc = ft_strdup(res + 1);
		if (truc[ft_strlen(truc) - 1] == '\'')
			truc[ft_strlen(truc) - 1] = 0;
		if (res)
			free(res);
		res = ft_strdup(truc);
		free(truc);
	}
	return (cut_reparse_dol(res, env, str));
}

int	dol_check(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	ft_echo(t_cmd *cmd, char **tab, t_env **env)
{
	int		i;
	int		j;

	if (!tab)
	{
		putstr_fd_echo("\n", 1);
		return ;
	}
	ft_suppr(tab, env);
	i = ft_check_options(tab);
	j = i;
	while (tab[i])
	{
		if (dol_check(tab[i]))
		{
			tab[i] = reparse_dol(tab[i], env);
		}
		putstr_fd_echo(tab[i], cmd->fd_out);
		i++;
		if (tab[i])
			ft_putchar_fd(' ', cmd->fd_out);
	}
	if (!j)
		ft_putchar_fd('\n', cmd->fd_out);
}
