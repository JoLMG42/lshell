/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:42:30 by lcalvie           #+#    #+#             */
/*   Updated: 2023/05/04 18:45:23 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	n;
	int	len;

	len = ft_strlen(to_find);
	i = 0;
	if (to_find[i] == '\0')
		return (str);
	while (str[i] != '\0')
	{
		n = 0;
		while (str[i + n] == to_find[n])
		{
			if (n == len - 1)
				return (&str[i]);
			n++;
		}
		i++;
	}
	return (0);
}

void	fill_start(char *recup, char **start, int *i, int *count2)
{
	while (recup[*i] && recup[*i] != ' ')
	{
		(*start)[*i] = recup[*i];
		count2--;
		(*i)++;
	}
	(*start)[*i] = 0;
	if (*i == 0)
	{
		free(*start);
		*start = NULL;
	}
	(*i)++;
}

int	fill_mid(char *recup, char **mid, int *count2, int count)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (recup[i] && recup[i] != ' ' && count > 1 && (*count2) > 1)
	{
		(*mid)[++j] = recup[i];
		(*count2)--;
		i++;
	}
	(*mid)[++j] = 0;
	if (j == 0)
	{
		free(*mid);
		*mid = NULL;
	}
	else
		i++;
	return (i);
}

void	fill_end(char *recup, char **end, int *i)
{
	int	j;

	j = -1;
	while (recup[*i] && recup[*i] != ' ')
	{
		(*end)[++j] = recup[*i];
		(*i)++;
	}
	(*end)[++j] = 0;
	if (j == 0)
	{
		free(*end);
		*end = NULL;
	}
}

int	find_count_wildcards(char *recup, char *file, int count, int count2)
{
	int		i;
	char	*start;
	char	*mid;
	char	*end;

	start = malloc(ft_strlen(recup) + 1);
	mid = malloc(ft_strlen(recup) + 1);
	end = malloc(ft_strlen(recup) + 1);
	i = 0;
	fill_start(recup, &start, &i, &count2);
	i += fill_mid(recup + i, &mid, &count2, count);
	fill_end(recup, &end, &i);
	count = 0;
	if (!start || (ft_strncmp(start, file, ft_strlen(start)) == 0))
		count++;
	if (!mid || ft_strstr(file, mid))
		count++;
	if (!end || (ft_cmpend(end, file) == 0))
		count++;
	free(start);
	free(mid);
	free(end);
	return (count);
}
