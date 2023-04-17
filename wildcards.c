/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:30:58 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/17 14:06:24 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checkstar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

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

int	checkfile(char *file, char *flag)
{
	int	i;
	int	j;
	int	h;
	int	n;
	char	**tab;
	char 	*recup;
	char	*start;
	char	*mid;
	char	*end;
	int	count;

	i = 0;
	j = 0;
	n = 0;
	h = 0;
	count = 0;
	recup = malloc(sizeof(char) * (ft_strlen(flag) + 10));
	while (flag[i])
	{
		if (file[0] == '.' && flag[0] != '.')
		{
			free(recup);
			return (0);
		}
		if (flag[i] != '*')
		{
			recup[j] = flag[i];
			j++;
		}
		else
		{
			count++;
			recup[j] = ' ';
			j++;
		}
		i++;
	}
	recup[j] = 0;
	start = malloc(i);
	mid = malloc(i);
	end = malloc(i);
	i = 0;
	while (recup[i] && recup[i] != ' ')
	{
		start[i] = recup[i];
		i++;
	}
	start[i] = 0;
	if (i == 0 && start[i] == 0)
	{
		free(start);
		start = NULL;
	}
	i++;
	while (recup[i] && recup[i] != ' ' && count > 1)
	{
		mid[n] = recup[i];
		n++;
		i++;
	}
	mid[n] = 0;
	if (n > 0)
		i++;
	while (recup[i] && recup[i] != ' ')
	{
		end[h] = recup[i];
		h++;
		i++;
	}
	end[h] = 0;
	if (n == 0 && mid[n] == 0)
	{
		free(mid);
		mid = NULL;
	}
	if (h == 0 && end[h] == 0)
	{
		free(end);
		end = NULL;
	}
	
	count = 0;
	if (start)
	{
		if (ft_strncmp(start, file, ft_strlen(start)) == 0)
			count++;
	}
	else
		count++;
	if (mid)
	{
		if (ft_strstr(file, mid))
			count++;
	}
	else
		count++;
	if (end)
	{
		if (ft_cmpend(end, file) == 0)
			count++;
	}
	else
		count++;
	free(recup);
	free(start);
	free(mid);
	free(end);
	if (count == 3)
		return (1);
	return (0);
	
}

char	**recreatearg(char **tab, char *str)
{
	struct	dirent	*file;
	DIR	*dir;
	char	**res;
	int	i;
	int	j;
	int	count;

	i = 0;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	file = readdir(dir);
	while (file)
	{
		i++;
		file = readdir(dir);
	}
//	free(dir);
	closedir(dir);
	/*dir = opendir(".");
	if (!dir)
	{
		//free(dir);
		return (NULL);
	}*/
	i = 0;
	j = 0;
	count = 0;
	res = malloc(sizeof(char *) * (i + 100));
	int len = tab_len(tab);
	while (tab[j])
	{
		if (checkstar(tab[j]))
		{
			dir = opendir(".");
			file = readdir(dir);
			while (file)
			{
				if (checkfile(file->d_name, str))
				{
					res[i] = ft_strdup(file->d_name);
					i++;
					count++;
				}
				file = readdir(dir);
			}
			closedir(dir);
		}
		if (ft_strcmp(tab[j], str) != 0 || count == 0)
		{
			res[i] = ft_strdup(tab[j]);
			i++;
		}
		count = 0;
		j++;
		if (len == j)
			break ;
	}
	//closedir(dir);
	//free(dir);
	res[i] = 0;
	return res;
}
void	setwildcardsfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	f;

	tmp = (*lst)->next;
	tab = malloc(sizeof(char *) * (tab_len(tmp->cmd_left->arg) + 1));
	i = 0;
	f = 0;
	while (tmp->cmd_left->arg[i])
	{
		if (checkstar(tmp->cmd_left->arg[i]))
		{
			free(tab);
			tab = recreatearg(tmp->cmd_left->arg, tmp->cmd_left->arg[i]);
			f = 1;
		}
		i++;
	}
	//free_tab(tmp->cmd_left->arg);
	if (f)
	{
		free_tab(tmp->cmd_left->arg);
		tmp->cmd_left->arg = tab;
	}
	else
			free(tab);
}

void	setwildcards(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	f;

	tab = NULL;
	tmp = (*lst)->next;
	while (tmp)
	{
		if (tmp->cmd_right->arg)
		{
			tab = malloc(sizeof(char *) * (tab_len(tmp->cmd_right->arg) + 1));
			i = 0;
			f = 0;
			while (tmp->cmd_right->arg[i])
			{
				if (checkstar(tmp->cmd_right->arg[i]))
				{
					tab = recreatearg(tmp->cmd_right->arg, tmp->cmd_right->arg[i]);
					f = 1;
				}
				i++;
			}
			if (f)
			{
				free_tab(tmp->cmd_right->arg);
				tmp->cmd_right->arg = tab;
			}
		}
		tmp = tmp->next;
	}
}
