/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supersplit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:51:38 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/10 16:30:19 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <linux/random.h>

#include "struct.h"
#include "minishell.h"


static int	ft_check_charset(const char str, char c, t_s *s_s)
{
	if (str == c && s_s->sq_opened == 0 && s_s->dq_opened == 0 && s_s->braces == 0)
		return (1);
	return (0);
}

static int	ft_count_word(const char *str, char c)
{
	int	i;
	int	count;
	t_s	s_s;

	init_syntax_struct(&s_s);
	count = 0;
	i = 0;
	while (str[i])
	{
		check_sq_dq(&s_s, str[i]);
		if (ft_check_charset(str[i], c, &s_s) == 0)
			i++;
		else
		{
			while (str[i] && ft_check_charset(str[i], c, &s_s) == 1)
				i++;
			count++;
		}
	}
	return (count);
}

static int	ft_lenword(const char *str, char c, t_s *s_s)
{
	int	i;

	i = 0;
	while (str[i] && ft_check_charset(str[i], c, s_s) == 0)
	{
		check_sq_dq(s_s, str[i]);
		i++;
	}
	return (i);
}

static char	*ft_cpy(const char *str, char c, int n, t_s *s_s)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_lenword(str, c, s_s) + 1));
	if (!cpy)
		return (0);
	while (str[i] && i < n)
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	**ft_supersplit(char *s, char c)
{
	int		i;
	int		j;
	char	**str;
	t_s		s_s;

	init_syntax_struct(&s_s);
	i = 0;
	j = 0;
	str = malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!str)
		return (0);
	while (s[i])
	{
		if (ft_check_charset(s[i], c, &s_s) == 1)
			i++;
		else
		{
			str[j] = ft_cpy(s + i, c, ft_lenword(s + i, c, &s_s), &s_s);
			j++;
			i = i + ft_lenword(s + i, c, &s_s);
		}
	}
	str[j] = 0;
	return (str);
}
