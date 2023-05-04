/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 18:03:37 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*exec_first_pass(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (!tmp->next)
		exec_no_next(tmp, env, exp, shell);
	else if (ft_strcmp(tmp->ope, "|") == 0)
		tw = exec_pp(tmp, tw, exp, shell);
	else if (ft_strcmp(tmp->ope, "&&") == 0)
		tw = exec_ee(tmp, tw, exp, shell);
	else if (ft_strcmp(tmp->ope, "||") == 0)
		tw = exec_ou(tmp, tw, exp, shell);
	return (tw);
}

t_tree	*exec_after(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell)
{
	t_env	**env;

	env = recup_struct_env2(NULL, 2);
	if (tmp->next && ft_strcmp(shell->saveope, "|") == 0
		&& ft_strcmp(tmp->ope, "|") == 0
		&& ft_strcmp(tmp->next->ope, "|") == 0)
		exec_sec_grp_pp(tmp, env, exp, shell);
	else if (ft_strcmp(shell->saveope, "|") == 0
		&& ft_strcmp(tmp->ope, "|") == 0)
		to_wait = exec_sec_pp_end(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "&&") == 0)
		to_wait = exec_sec_grp_ee(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "||") == 0)
		to_wait = exec_sec_grp_ou(tmp, to_wait, exp, shell);
	else if (ft_strcmp(tmp->ope, "|") == 0)
		to_wait = exec_sec_grp_pp_2(tmp, to_wait, exp, shell);
	return (to_wait);
}

t_tree	*exec_end_while(t_tree *tmp, t_shell *shell)
{
	tmp = skip_pipe(tmp, shell);
	if (tmp && tmp->cmd_left->is_hd)
	{
		if (tmp->cmd_left->name_in)
			unlink(tmp->cmd_left->name_in);
	}
	if (tmp && tmp->cmd_right->is_hd)
	{
		if (tmp->cmd_right->name_in)
			unlink(tmp->cmd_right->name_in);
	}
	if (shell->saveope)
	{
		free(shell->saveope);
		shell->saveope = NULL;
	}
	shell->saveope = ft_strdup(tmp->ope);
	tmp = tmp->next;
	return (tmp);
}

int	mini_init(t_shell *shell, t_tree *to_wait)
{
	to_wait = NULL;
	pipe(shell->pipefd);
	signal(SIGINT, handler_fork);
	signal(SIGQUIT, handler_fork);
	return (0);
}

void	exec(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;
	int		i;
	t_tree	*to_wait;

	to_wait = NULL;
	init_heredoc(tree, env, exp, shell);
	tmp = (*tree)->next;
	(*tree)->next->in_exec = 1;
	i = mini_init(shell, to_wait);
	while (tmp)
	{
		if (!tmp->cmd_right->cmd)
			return (exec_one_only(tmp, env, exp, shell));
		if (i == 0)
			to_wait = exec_first_pass(tmp, to_wait, exp, shell);
		else
			to_wait = exec_after(tmp, to_wait, exp, shell);
		i++;
		tmp = exec_end_while(tmp, shell);
	}
	if (shell && shell->saveope)
	{
		free(shell->saveope);
		shell->saveope = NULL;
	}
}
