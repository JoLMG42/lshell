/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:30:58 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:58:07 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[0] == 0)
		return ((char *)haystack);
	while (i < len && haystack[i])
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (haystack[j + i] == needle[j] && needle[j] && j + i < len)
			{
				j++;
				if (!needle[j] && haystack[j + i])
					return ((char *)haystack + i);
			}
		}
		i++;
	}
	return (NULL);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && i < n)
	{
		s1++;
		s2++;
		i++;
	}
	if (i < n)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	return (0);
}

int	ft_cmpend(char *s1, char *s2)
{
	int	i;
	int	j;

	j = 0;
	while (s1[j])
		j++;
	i = 0;
	while (s2[i])
		i++;
	j--;
	i--;
	while (j >= 0)
	{
		if (s1[j] != s2[i])
			return (1);
		i--;
		j--;
	}
	return (0);
}

char	**recreatearg(char **tab, char *str)
{
	char			**res;
	int				i;
	int				j;
	int				count;

	if (!count_size_dir())
		return (NULL);
	res = malloc(sizeof(char *) * (count_size_dir() + 100));
	i = 0;
	j = -1;
	while (tab[++j])
	{
		count = 0;
		if (checkstar(tab[j]))
			check_all_files(str, res, &i, &count);
		if (ft_strcmp(tab[j], str) != 0 || count == 0)
		{
			res[i] = ft_strdup(tab[j]);
			i++;
		}
		if (tab_len(tab) == j)
			break ;
	}
	res[i] = 0;
	return (res);
}

void	check_wildcards_first_cmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**res;
	int		i;
	int		f;
	char	**split;

	tmp = (*lst)->next;
	split = NULL;
	if (tmp->cmd_left->cmd)
		split = ft_supersplit(tmp->cmd_left->cmd, ' ');
	i = 0;
	f = 0;
	if (checkstar(split[0]))
	{
		res = recreatearg(&split[0], split[0]);
		f = 1;
	}
	if (f)
	{
		free(tmp->cmd_left->cmd);
		tmp->cmd_left->cmd = ft_strdup(*res);
		free_tab(res);
	}
	if (split)
		free_tab(split);
}
