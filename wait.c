/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:44:13 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 22:02:07 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait(t_cmd **cmd)
{
	int		value;
	t_cmd	*cmd_lst;

	value = 0;
	cmd_lst = *cmd;
	if (!cmd_lst)
		return ;
	waitpid(cmd_lst->pid, &value, 0);
	if (cmd_lst->cmd && is_builtins(cmd_lst->cmd))
		return ;
	if (WIFSIGNALED(value))
		g_rvalue = (WTERMSIG(value) + 128);
	else
		g_rvalue = WEXITSTATUS(value);
}

void	ft_wait_all(t_tree *to_wait, t_tree *end, int first)
{
	t_tree	*tmp;

	tmp = to_wait;
	while (tmp != end)
	{
		if (tmp == to_wait && first)
			ft_wait(&(tmp->cmd_left));
		ft_wait(&(tmp->cmd_right));
		tmp = tmp->next;
	}
	if (tmp == to_wait && first)
		ft_wait(&(tmp->cmd_left));
	ft_wait(&(tmp->cmd_right));
}

void	braces_enginer(char *cmd, t_env **env, t_env **exp)
{
	char	*cpy;
	t_tree	*tree;
	int		frk;

	recup_status_braces(9);
	frk = fork();
	if (frk == 0)
	{
		cpy = ft_strdup(cmd);
		pars_prompt(cpy, *env, *exp, 2);
		ft_lstclear_env(env, del);
		ft_lstclear_env(exp, del);
		tree = recup_struct(NULL, 1);
		ft_lstcleartree(&tree, del);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		exit(0);
	}
	else
		waitpid(frk, 0, -1);
}

int	recup_status_braces(int i)
{
	static int	n;

	if (i == 9)
		n = i;
	if (i == 0)
		return (n);
	if (i == -1)
		n = i;
	return (0);
}
