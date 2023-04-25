/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:42:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/25 13:46:03 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_mega_atoi(char *str)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i])
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

void	ft_exit_error_1(char **split, char *line, char *tmp)
{
	(void)line;
	if (!tmp)
	{
		printf("Minishell: exit: %s: numeric argument required\n", split[0]);
		printf("Bye! 👋\n");
		//ft_lstclear(&v->list, &free);
		//ft_free(v);
		free_tab(split);
		exit(2);
	}
	else
	{
		printf("exit\nMinishell: exit: too many arguments\n");
		g_rvalue = 1;
		free_tab(split);
		free(tmp);
	}
}

void	ft_error_exit_2(char **split, char *tmp)
{
	printf("Bye! 👋\n");
	printf("Minishell: exit: %s: numeric argument required\n", split[0]);
	g_rvalue = 1;
	//ft_lstclear(&v->list, &free);
	free_tab(split);
	//ft_free(v);
	if (tmp)
		free(tmp);
	exit (2);
}

static void	ft_exit_2(char **split, int valeur, char *tmp)
{
	printf("Bye! 👋\n");
	free(tmp);
	free_tab(split);
	//ft_lstclear(&v->list, &free);
	//ft_free(v);
	exit(valeur);
}

void	ft_exit(t_cmd *cmd)
{
	long long	valeur;
	char		*tmp;
	char		**split;
	int			i;
	char		*line;
	
	if (!cmd->arg[0])
	{
		//ft_lstclear(env, del);
		exit(g_rvalue);
	}
	line = rejoinstr(cmd->arg);
	split = ft_supersplit(line, ' ');
	i = -1;
	while (split[0][++i])
		if (ft_isalpha(split[0][i]))
			ft_exit_error_1(split, line, NULL);
	valeur = ft_mega_atoi(line);
	tmp = ft_itoa(valeur);
	if (split[1] != NULL)
		ft_exit_error_1(split, line, tmp);
	else if (ft_strcmp(line, tmp))
		ft_error_exit_2(split, tmp);
	else
		ft_exit_2(split, valeur, tmp);
}
