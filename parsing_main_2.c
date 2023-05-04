/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:58:02 by lcalvie           #+#    #+#             */
/*   Updated: 2023/05/04 15:58:11 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell_struct(t_shell *shell)
{
	shell->tmpfd = 0;
	shell->skip = 0;
	shell->saveope = NULL;
	shell->first_to_wait = 1;
	shell->env = NULL;
	shell->exp = NULL;
	shell->tree = NULL;
}

t_shell	*recup_shell(t_shell *shell)
{
	static t_shell	*tmp;

	if (!shell)
		return (tmp);
	if (shell)
		tmp = shell;
	return (NULL);
}

char	**pars_first_check(char *str, char **tab)
{
	char	*recup;

	recup = add_spaces(str, 0, 0);
	if (!recup)
	{
		g_rvalue = 2;
		return (0);
	}
	tab = ft_supersplit(recup, ' ');
	free(recup);
	if (!tab[0])
	{
		g_rvalue = 667;
		free_tab(tab);
		return (0);
	}
	if (!check_syntax(tab))
	{
		g_rvalue = 2;
		free_tab(tab);
		return (0);
	}
	return (tab);
}

void	init_shell_and_tab(char *str, t_shell **shell, char ***tab)
{
	*shell = malloc(sizeof(struct s_shell));
	init_shell_struct(*shell);
	recup_shell(*shell);
	*tab = pars_first_check(str, NULL);
	if (!(*tab))
		free(*shell);
	else
		*tab = rejointab(*tab);
}

void	debug_print(t_tree *tree)
{
	t_tree	*test;
	int		n;

	test = tree->next;
	while (test)
	{
		printf("			OPERATOR: %s\n", test->ope);
		printf("CMD LEFT: %s\n", test->cmd_left->cmd);
		n = 0;
		if (test->cmd_left->arg)
		{
			while (test->cmd_left->arg[n])
			{
				printf("ARG A LA CMD: %s\n", test->cmd_left->arg[n]);
				n++;
			}
		}
		printf("REDIR IN: %s\nREDIR OUT: %s\nIS_HD: %d\nLIMITER: %s\nBRACE LVL: %d\n\n\n\n", test->cmd_left->name_in, test->cmd_left->name_out, test->cmd_left->is_hd, test->cmd_left->limiter, test->cmd_left->bracelvl);
		printf("CMD RIGHT: %s\n", test->cmd_right->cmd);
		n = 0;
		if (test->cmd_right->arg)
		{
			while (test->cmd_right->arg[n])
			{
				printf("ARG A LA CMD: %s\n", test->cmd_right->arg[n]);
				n++;
			}
		}
		printf("REDIR IN: %s\nREDIR OUT: %s\nIS_HD: %d\nLIMITER: %s\nBRACE LVL: %d\n\n\n\n", test->cmd_right->name_in, test->cmd_right->name_out, test->cmd_right->is_hd, test->cmd_right->limiter, test->cmd_right->bracelvl);
		test = test->next;
	}
}
