/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:50:43 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:49:26 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_while(char **tab, t_tree *tree, int *i)
{
	*i = 1;
	if (!tab[1])
	{
		ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
				ft_lstnew(tab[0]), ft_lstnew(NULL)));
	}
	while (tab && tab[*i])
	{
		if ((ft_strcmp(tab[*i], "&&") == 0) || (ft_strcmp(tab[*i], "||") == 0)
			|| (ft_strcmp(tab[*i], "|") == 0))
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(tab[*i],
					ft_lstnew(tab[*i - 1]), ft_lstnew(tab[*i + 1])));
			(*i) += 2;
		}
		else
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
					ft_lstnew(tab[*i]), ft_lstnew(NULL)));
			(*i)++;
		}
	}
}

void	parse_first_cmd(t_tree *tree, t_env *env)
{
	parsefirstcmd(&tree, &env, NULL);
	setargfirstcmd(&tree);
	ouverturefirstcmd(&tree);
	setbracelvlfirstcmd(&tree);
	setwildcardsfirstcmd(&tree);
	parseargfirstcmd(&tree, NULL, NULL);
}

void	parse_other_cmd(t_tree *tree, t_env *env)
{
	parsecmd(&tree, &env, NULL);
	setarg(&tree);
	ouvertureredir(&tree);
	setbracelvl(&tree);
	setwildcards(&tree);
	parsearg(&tree, &env);
}

void	start_exec(t_tree *tree, t_shell *shell, t_env *env, t_env *exp)
{
	t_tree		*u;

	recup_struct(&tree, 0);
	shell->tree = tree;
	shell->env = env;
	shell->exp = exp;
	exec(&tree, &env, &exp, shell);
	ft_lstcleartree(&tree, del);
	u = recup_struct(NULL, 10);
	ft_lstcleartree(&u, del);
	recup_struct(NULL, 3);
}

int	pars_prompt(char *str, t_env *env, t_env *exp, int mode)
{
	char		**tab;
	static int	flag;
	t_tree		*tree;
	t_shell		*shell;
	int			i;

	if (mode == 5)
		flag = 1;
	if (mode == 2)
	{
		flag = 0;
	}
	init_shell_and_tab(str, &shell, &tab);
	if (!tab)
		return (g_rvalue == 667);
	tree = ft_lstnewtree(NULL, NULL, NULL);
	parse_while(tab, tree, &i);
	free_tab(tab);
	parse_first_cmd(tree, env);
	if (i > 1)
		parse_other_cmd(tree, env);
	debug_print(tree);
	start_exec(tree, shell, env, exp);
	free(shell);
	return (1);
}
