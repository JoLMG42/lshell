/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:51:21 by lcalvie           #+#    #+#             */
/*   Updated: 2023/05/04 18:58:52 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_recup(char *file, char *flag, int *count, int *count2)
{
	int		i;
	int		j;
	char	*recup;

	i = -1;
	j = -1;
	recup = malloc(sizeof(char) * (ft_strlen(flag) + 10));
	if (!recup)
		return (NULL);
	while (flag[++i])
	{
		if (file[0] == '.' && flag[0] != '.')
			return (free(recup), NULL);
		if (flag[i] != '*')
			recup[++j] = flag[i];
		else if ((i > 0 && flag[i - 1] != flag[i]) || (i == 0))
		{
			(*count)++;
			(*count2)++;
			recup[++j] = ' ';
		}
	}
	recup[++j] = 0;
	return (recup);
}

int	checkfile_all_in(char *file, char *flag)
{
	char	**split;
	int		i;
	int		count;
	char	*str;

	split = ft_supersplit(flag, ' ');
	i = 0;
	count = 0;
	while (split[i])
	{
		if (i == 0)
			str = ft_strstr(file, split[i]);
		else
			str = ft_strstr(str, split[i]);
		if (str)
			count++;
		i++;
	}
	free_tab(split);
	free(flag);
	if (i == count)
		return (1);
	return (0);
}

int	checkfile(char *file, char *flag)
{
	char	*recup;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	recup = init_recup(file, flag, &count, &count2);
	if (!recup)
		return (0);
	if (flag[0] == '*' && flag[ft_strlen(flag) - 1] == '*')
		return (checkfile_all_in(file, recup));
	count = find_count_wildcards(recup, file, count, count2);
	free(recup);
	return (count == 3);
}

int	count_size_dir(void)
{
	struct dirent	*file;
	DIR				*dir;
	int				i;

	i = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	file = readdir(dir);
	while (file)
	{
		i++;
		file = readdir(dir);
	}
	closedir(dir);
	return (i);
}

void	check_all_files(char *str, char **res, int *i, int *count)
{
	struct dirent	*file;
	DIR				*dir;

	dir = opendir(".");
	file = readdir(dir);
	while (file)
	{
		if (checkfile(file->d_name, str))
		{
			res[*i] = ft_strdup(file->d_name);
			(*i)++;
			(*count)++;
		}
		file = readdir(dir);
	}
	closedir(dir);
}
