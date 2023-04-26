/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:44:25 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 14:44:19 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	ft_env(t_cmd *cmd, t_env **env)
{
	t_env	*tmp;

	tmp = (*env)->next;
	while (tmp)
	{
		if (tmp->line)
		{
			putstr_fd_echo(tmp->line, cmd->fd_out);
			ft_putchar_fd('\n', cmd->fd_out);
		}
		tmp = tmp->next;
	}
}
