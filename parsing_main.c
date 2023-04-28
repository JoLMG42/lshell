/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:50:43 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 17:59:01 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**pars_first_check(char *recup, char **tab)
{
	if (!recup)
	{
		g_rvalue = 2;
		return (0);
	}
	tab = NULL;
	tab = ft_supersplit(recup, ' ');
	free(recup);
	if (!tab[0])
	{
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

void	pars_recup_ope(t_tree **tree, char **tab, int i, char *t)
{
	char	*o;
	char	*p;

	o = NULL;
	p = NULL;
	if (!tab[1])
	{
		t = ft_strdup(tab[0]);
		ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
			ft_lstnew(t), ft_lstnew(NULL)));
		free(t);
	}
	while (tab && tab[i])
	{
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			t = ft_strdup("&&");
			o = ft_strdup(tab[i - 1]);
			p = ft_strdup(tab[i + 1]);
			ft_lstadd_backtree(&tree, ft_lstnewtree(t,
				ft_lstnew(o), ft_lstnew(p)));
			i += 2;
			free(t);
			free(o);
			free(p);
		}
		else if (ft_strcmp(tab[i], "||") == 0)
		{
			t = ft_strdup("||");
			o = ft_strdup(tab[i - 1]);
			p = ft_strdup(tab[i + 1]);
			ft_lstadd_backtree(&tree, ft_lstnewtree(t,
				ft_lstnew(o), ft_lstnew(p)));
			i += 2;
			free(t);
			free(o);
			free(p);
		}
		else if (ft_strcmp(tab[i], "|") == 0)
		{
			t = ft_strdup("|");
			o = ft_strdup(tab[i - 1]);
			p = ft_strdup(tab[i + 1]);
			ft_lstadd_backtree(&tree, ft_lstnewtree(t,
				ft_lstnew(o), ft_lstnew(p)));
			i += 2;
			free(t);
			free(o);
			free(p);
		}
		else
		{
			o = ft_strdup(tab[i]);
			ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
				ft_lstnew(o), ft_lstnew(NULL)));
			i++;
			free(o);
		}
	}
}

int	pars_prompt(char *str, t_env *env, t_env *exp)
{
	char	*recup;
	char	**tab;
	t_cmd	*l_cmd;
	t_tree	*tree;
	t_shell	*shell;

	recup = add_spaces(str, -1, 0);
	tab = pars_first_check(recup, tab);
	if (!tab)
		return (0);
	shell = malloc(sizeof(struct s_shell));
	shell->env = NULL;
	shell->exp = NULL;
	shell->tree = NULL;
	int	i = 0;
	int	c = 0;
	tab = rejointab(tab);
	tree = ft_lstnewtree(NULL, NULL, NULL);
	pars_recup_ope(&tree, tab, 1, NULL);
	free_tab(tab);
	t_env	*tmp_env = env;
	t_tree	*test = tree->next;
	parsefirstcmd(&tree, &env);
	setargfirstcmd(&tree);
	ouverturefirstcmd(&tree);
	setbracelvlfirstcmd(&tree);
	setwildcardsfirstcmd(&tree);
	parseargfirstcmd(&tree, &env);
	if (i > 1)
	{
		parsecmd(&tree, &env);
		setarg(&tree);
		ouvertureredir(&tree);
		setbracelvl(&tree);
		setwildcards(&tree);
		parsearg(&tree, &env);
	}
	while (test)
	{
		printf("			OPERATOR: %s\n", test->ope);
		printf("CMD LEFT: %s\n", test->cmd_left->cmd);
		int n = 0;
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
	recup_struct(&tree, 0);
	shell->tree = tree;
	shell->env = env;
	shell->exp = exp;
	exec(&tree, &env, &exp, shell);


	ft_lstcleartree(&tree, del);
	recup_struct(NULL, 3);
	free(shell);
	return (1);
}

