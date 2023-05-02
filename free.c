/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 18:58:24 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/02 16:58:47 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tree;
	t_tree	*toto;

	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
	free(shell->saveope);
	free(shell);
	tree = recup_struct(NULL, 1);
//	toto = recup_struct(NULL, 10);
	ft_lstcleartree(&tree, del);
//	ft_lstcleartree(&toto, del);
}
