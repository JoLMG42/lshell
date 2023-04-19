/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 14:44:25 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/19 22:25:16 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	ft_env(t_env **env)
{
	t_env	*tmp;

	tmp = (*env)->next;
	while (tmp)
	{
		if (tmp->line)
			printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
}
