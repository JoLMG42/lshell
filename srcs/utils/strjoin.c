/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 11:19:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 17:28:14 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin5(char *s1, char *s2, char *join)
{
	int	i;

	i = 0;
	while (*s1)
	{
		join[i] = *s1;
		s1++;
		i++;
	}
	while (*s2)
	{
		join[i] = *s2;
		s2++;
		i++;
	}
	join[i] = 0;
	return (join);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		total_lenght;
	char	*join;

	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
	}
	if (!s2 && s1)
		return (s1);
	if (!s1 || !s2)
		return (NULL);
	total_lenght = (ft_strlen(s1) + ft_strlen(s2) + 1);
	join = malloc(sizeof(char *) * total_lenght);
	if (!join)
		return (NULL);
	join = ft_strjoin5(s1, s2, join);
	free(s1);
	return (join);
}

char	*ft_strjoin2(char *s1, char *s2)
{
	int		total_lenght;
	char	*join;
	int		c;

	c = 0;
	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
		c = 1;
	}
	if (!s1 || !s2)
		return (NULL);
	total_lenght = (ft_strlen(s1) + ft_strlen(s2) + 1);
	join = malloc(sizeof(char *) * total_lenght);
	if (!join)
		return (NULL);
	join = ft_strjoin5(s1, s2, join);
	if (c == 1)
		free(s1);
	return (join);
}

void	exec_fail(char *cmd)
{
	putstr_fd_echo("minishell: ", 2);
	if (!cmd)
		putstr_fd_echo("", 2);
	else
		putstr_fd_echo(cmd, 2);
	putstr_fd_echo(": command not found\n", 2);
}
