/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:44:22 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 17:35:26 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_name(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (0);
}

char	*reallocator(char *res, char c)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = NULL;
	if (!res)
	{
		res = malloc(2);
		res[0] = c;
		res[1] = 0;
		return (res);
	}
	if (res)
	{
		cpy = ft_strdup(res);
		free(res);
	}
	res = malloc(sizeof(char) * (ft_strlen(cpy) + 2));
	while (cpy && cpy[i])
	{
		res[i] = cpy[i];
		i++;
	}
	res[i] = c;
	res[i + 1] = 0;
	return (res);
}

char	*expander(char *str, t_env **env, int i, char *res)
{
	if (!str || !str[i])
		return (str);

	(void)res;
	int	c = 0;
	int	j;
	int	sq = 0;
	int	dq = 0;
	int	f = 0;
	//res = malloc(sizeof(char) * (ft_strlen(str) + 1);
	char	*recup = NULL;
	while (str && str[i])
	{
		recup = NULL;
		if (str[i] == '\'' && sq == 0 && dq == 1)
			;
		else if (str[i] == '\'' && sq == 0 && dq == 0)
			sq = 1;
		else if (str[i] == '\'' && sq == 1)
			sq = 0;
		else if (str[i] == '\"' && dq == 0 && sq == 1)
			;
		else if (str[i] == '\"' && dq == 0 && sq == 0)
			dq = 1;
		else if (str[i] == '\"' && dq == 1)
			dq = 0;
		if (str[i] == '$' && sq == 0)
		{
			j = 0;
			i++;
			c++;
			while (str && str[i] && str[i] != ' ' && valid_name(str[i]) && str[i] != '$')
			{
				recup = reallocator(recup, str[i]);;
				i++;
			}
			if (str[i] == '$' || str[i] == ' ' || !valid_name(str[i]))
				i--;
			if (recup)
				res = ft_strjoin(res, recup_content_env2(recup, env));
			(void)env;
		}
		else
		{
			f = 1;
			res = reallocator(res, str[i]);
		} 
		if (!recup && f == 0)
			res = reallocator(res, str[i]);
		i++;
		c++;
		f = 0;
	}
	printf("REEEEESSS = %s\n", res);
	return (res);
}
