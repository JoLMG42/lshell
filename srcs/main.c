/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 12:29:11 by jtaravel         ###   ########.fr       */
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

int	main_while(t_env *env, t_env *exp)
{
	char	*str;
	int		i;

	str = NULL;
	i = recup_status_braces(0);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	if (i != 9)
		str = readline("ft_containers$ ");
	else
	{
		str = readline("");
		recup_status_braces(-1);
	}
	if (!str)
		return (0);
	if (!pars_prompt(str, env, exp, 5))
		printf("INVALID SYNTAX\n");
	if (g_rvalue == 667)
		g_rvalue = 0;
	if (str[0])
		add_history(str);
	free(str);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_env	*exp;

	(void)ac;
	(void)av;
	if (!ttyname(1) || !ttyname(0))
		return (0);
	get_env(&env, envp, 0, &exp);
	recup_struct_env(&env, 0);
	recup_struct_env(&exp, 5);
	recup_struct_env2(&env, 1);
	recup_struct_env2(&exp, 5);
	while (1)
	{
		if (!main_while(env, exp))
			break ;
	}
	printf("\n");
	free_stuff(&env, &exp);
}
