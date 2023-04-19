/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:48:20 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/19 14:49:17 by jtaravel         ###   ########.fr       */
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

static void	ft_suppr(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		ft_suppr_dq_sq(tab[i]);
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

void	ft_echo(char **tab)
{
	int		i;
	int		j;

	if (!tab)
	{
		putstr_fd_echo("\n", 1);
		return ;
	}
	ft_suppr(tab);
	i = ft_check_options(tab);
	j = i;
	while (tab[i])
	{
		putstr_fd_echo(tab[i], 1);
		i++;
		if (tab[i])
			ft_putchar_fd(' ', 1);
	}
	if (!j)
		ft_putchar_fd('\n', 1);
}
