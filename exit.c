/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:38:19 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 19:38:21 by jtaravel         ###   ########.fr       */
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

void	ft_exit_error_1(char **split, char *line, char *tmp, int mode)
{
	(void)line;
	if (!tmp || mode == 1)
	{
		printf("Minishell: exit: %s: numeric argument required\n", split[0]);
		printf("Bye! 👋\n");
		free(line);
		free_tab(split);
		exit(2);
	}
	else if (mode == 0)
	{
		printf("exit\nMinishell: exit: too many arguments\n");
		free(line);
		line = NULL;
		g_rvalue = 1;
		free_tab(split);
		split = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	ft_error_exit_2(char **split, char *tmp)
{
	printf("Bye! 👋\n");
	printf("Minishell: exit: %s: numeric argument required\n", split[0]);
	g_rvalue = 1;
	free_tab(split);
	if (tmp)
		free(tmp);
	exit (2);
}

void	ft_exit_2(char **split, int valeur, char *tmp)
{
	printf("Bye! 👋\n");
	free(tmp);
	free_tab(split);
	exit(valeur);
}

void	ft_exit(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell)
{
	char		**split;
	int			i;
	char		*line;

	if (!cmd->arg[0])
	{
		free_all(env, exp, shell);
		exit(g_rvalue);
	}
	line = rejoinstr(cmd->arg);
	split = ft_supersplit(line, ' ');
	i = -1;
	while (split[0][++i])
	{
		if (ft_isalpha(split[0][i]))
		{
			free_all(env, exp, shell);
			ft_exit_error_1(split, line, NULL, 0);
		}
	}
	if (split)
		free_tab(split);
	if (line)
		free(line);
	cut_exit(env, exp, shell, cmd);
}
