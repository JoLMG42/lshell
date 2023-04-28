/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 18:58:24 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/28 19:01:22 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tree;

	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
	free(shell->saveope);
	free(shell);
	tree = recup_struct(NULL, 1);
	ft_lstcleartree(&tree, del);
}
