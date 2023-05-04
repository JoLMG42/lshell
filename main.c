/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 15:49:06 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_rvalue = 0;

void	handler(int sig)
{
	t_tree	*tmp;

	tmp = recup_struct(NULL, 2);
	if (tmp && tmp->in_exec)
		return ;
	else if (sig == 2 && tmp && !tmp->in_exec)
	{
		rl_on_new_line();
		printf("\n");
		rl_replace_line("", 1);
		rl_redisplay();
		g_rvalue = 130;
		return ;
	}
	else if (!tmp && sig == 2)
	{
		rl_on_new_line();
		printf("\n");
		rl_replace_line("", 1);
		rl_redisplay();
		g_rvalue = 130;
		return ;
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*str;
	t_shell	*shell;
	t_env	*env;
	t_env	*exp;

	get_env(&env, envp, 0, &exp);
	recup_struct_env(&env, 0);
	recup_struct_env(&exp, 5);
	recup_struct_env2(&env, 1);
	recup_struct_env2(&exp, 5);
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("ft_containers$ ");
		if (!str)
			break ;
		if (!pars_prompt(str, env, exp, 5))
			printf("INVALID SYNTAX\n");
		if (str[0])
			add_history(str);
		free(str);
	}
	printf("\n");
	free_stuff(&env, &exp);
}
