/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:50:43 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 16:02:20 by jtaravel         ###   ########.fr       */
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
void	parse_while(char **tab, t_tree **tree)
{
	int	i = 1;
	char	*t, *o, *p;
	while (tab && tab[i])
	{

		if (ft_strcmp(tab[i], "&&") == 0)
		{
			t = ft_strdup("&&");
			o = ft_strdup(tab[i - 1]);
			p = ft_strdup(tab[i + 1]);
			ft_lstadd_backtree(tree, ft_lstnewtree(t,
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
			ft_lstadd_backtree(tree, ft_lstnewtree(t,
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
			ft_lstadd_backtree(tree, ft_lstnewtree(t,
				ft_lstnew(o), ft_lstnew(p)));
			i += 2;
			free(t);
			free(o);
			free(p);
		}
		else
		{
			o = ft_strdup(tab[i]);
			ft_lstadd_backtree(tree, ft_lstnewtree(NULL,
				ft_lstnew(o), ft_lstnew(NULL)));
			i++;
			free(o);
		}
	}
}

t_shell	*recup_shell(t_shell *shell)
{
	static	t_shell *tmp;

	if (!shell)
		return (tmp);
	if (shell)
		tmp = shell;
	return (NULL);
}

int	pars_prompt(char *str, t_env *env, t_env *exp, int mode)
{
	char	*recup;
	char	**tab;
	static	int	flag;
	t_cmd	*l_cmd;
	t_tree	*tree;
	t_shell	*shell;

	if (mode == 5)
		flag = 1;
	if (mode == 2)
	{
	//	tree = recup_struct(NULL, 1);
	//	shell = recup_shell(NULL);
//		free(shell);
		//ft_lstcleartree(&tree, del);
//		shell = NULL;
//		tree = NULL;
		flag = 0;
	}
//	if (mode != 2)
//	{
		shell = malloc(sizeof(struct s_shell));
		shell->env = NULL;
		shell->exp = NULL;
		shell->tree = NULL;
		recup_shell(shell);
//	}
	recup = add_spaces(str, 0, 0);
	//if (mode == 2)
	//	free(str);
	tab = pars_first_check(recup, NULL);
	if (!tab)
	{
		free(shell);
		if (g_rvalue == 667)
			return (1);
		return (0);
	}
	int	i = 0;
	int	c = 0;
	tab = rejointab(tab);
	i = 1;
	tree = ft_lstnewtree(NULL, NULL, NULL);
	char	*t;
	char	*o;
	char	*p;
	if (!tab[1])
	{
		t = ft_strdup(tab[0]);
		ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
			ft_lstnew(t), ft_lstnew(NULL)));
		free(t);
	}
	//parse_while(tab, &tree);
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
	free_tab(tab);
	t_env	*tmp_env = env;
	t_tree	*test = tree->next;
	parsefirstcmd(&tree, &env, NULL);
	setargfirstcmd(&tree);
	ouverturefirstcmd(&tree);
	setbracelvlfirstcmd(&tree);
	setwildcardsfirstcmd(&tree);
	parseargfirstcmd(&tree, &env);
	if (i > 1)
	{
		parsecmd(&tree, &env, NULL);
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
	//recup_struct(NULL, 11);
	shell->tree = tree;
	shell->env = env;
	shell->exp = exp;
	exec(&tree, &env, &exp, shell);


//	if (flag == 1)
//	{
		ft_lstcleartree(&tree, del);
		t_tree *u = recup_struct(NULL, 10);
		ft_lstcleartree(&u, del);
		recup_struct(NULL, 3);
		free(shell);
//	}
	return (1);
}


