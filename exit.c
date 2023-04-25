/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:42:39 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 01:47:50 by jtaravel         ###   ########.fr       */
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
		//ft_lstclear(&v->list, &free);
		//ft_free(v);
		free(line);
		free_tab(split);
		exit(2);
	}
	else if (mode == 0)
	{
		printf("exit\nMinishell: exit: too many arguments\n");
		free(line);
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

void	ft_exit(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell)
{
	long long	valeur;
	char		*tmp;
	char		**split;
	int			i;
	char		*line;
	t_tree 		*tree;
	
	if (!cmd->arg[0])
	{
		ft_lstclear_env(env, del);
		ft_lstclear_env(exp, del);
		free(shell->saveope);
		free(shell);
		tree = recup_struct(NULL, 1);
		ft_lstcleartree(&tree, del);
		exit(g_rvalue);
	}
	line = rejoinstr(cmd->arg);
	split = ft_supersplit(line, ' ');
	i = -1;
	while (split[0][++i])
	{
		if (ft_isalpha(split[0][i]))
		{
			ft_lstclear_env(env, del);
			ft_lstclear_env(exp, del);
			free(shell->saveope);
			free(shell);
			tree = recup_struct(NULL, 1);
			ft_lstcleartree(&tree, del);
			ft_exit_error_1(split, line, NULL, 0);
		}
	}
	valeur = ft_mega_atoi(line);
	tmp = ft_itoa(valeur);
	if (split[1] != NULL)
		ft_exit_error_1(split, line, tmp, 0);
	else if (ft_strcmp(line, tmp))
		ft_error_exit_2(split, tmp);
	else
	{
		ft_lstclear_env(env, del);
		ft_lstclear_env(exp, del);
		free(shell->saveope);
		free(shell);
		tree = recup_struct(NULL, 1);
		ft_lstcleartree(&tree, del);
		free(line);
		ft_exit_2(split, valeur, tmp);
	}
} 
