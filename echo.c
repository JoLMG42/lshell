/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:48:20 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 15:05:31 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static char	*cut_sds(char *str, t_s *s, int i, int c)
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

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

char	*ft_suppr_dq_sq(char *str)
{
	int	i;
	t_s	*s;

	s = malloc(sizeof(t_s));
	init_syntax_struct(s);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' && s->dq_opened == 0)
		{
			str = cut_sds(str, s, i, 1);
			i--;
		}
		else if (str[i] == '"' && s->sq_opened == 0)
		{
			str = cut_sds(str, s, i, 2);
			i--;
		}
	}
	free(s);
	return (str);
}

char    *reparse_dol(char *str, t_env **env);


static void	ft_suppr(char **tab, t_env **env)
{
	int		i;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		tmp = ft_strdup(tab[i]);
		tab[i] = reparse_dol(tab[i], env);
		if (ft_strcmp(tab[i], tmp) == 0)
			ft_suppr_dq_sq(tab[i]);
		free(tmp);
		i++;
	}
}

int	ft_check_options(char **tab)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i])
	{
		j = 0;
		if (tab[i][j] && tab[i][j++] == '-' && tab[i][j] == 'n')
		{
			while (tab[i][j] == 'n')
				j++;
			if (tab[i][j] && tab[i][j] != 'n')
				return (i);
		}
		else
			return (i);
		i++;
	}
	return (i);
}

void	putstr_fd_echo(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

char	*reparse_dol(char *str, t_env **env)
{
	char	*res;
	t_env	*tmp;
	int	i;
	char	*truc;

	i = 0;
	if (str[0] != '\'')
		return (str);
	//res = malloc(sizeof(char) * ft_strlen(str) + 1);
	res = ft_strdup(str + 1);
	res[ft_strlen(res) - 1] = 0;
	if (res[0] == '$')
	{
		truc = ft_strdup(res + 1);
		if (truc[ft_strlen(truc) - 1] == '\'')
			truc[ft_strlen(truc) - 1] = 0;
		res = ft_strdup(truc);
		free(truc);
	}
	tmp = var_in_exp(res, env);
	free(res);
	res = NULL;
	if (!tmp)
	{
		return (str);
	}
	res = ft_strjoin(res, "'");
	res = ft_strjoin(res, tmp->content);
	res = ft_strjoin(res, "'");
	return (res);
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
			reparse_dol(tab[i], env);
		}
			
		putstr_fd_echo(tab[i], cmd->fd_out);
		i++;
		if (tab[i])
			ft_putchar_fd(' ', cmd->fd_out);
	}
	if (!j)
		ft_putchar_fd('\n', cmd->fd_out);
}
