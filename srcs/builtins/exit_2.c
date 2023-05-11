/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:36:24 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/11 12:59:59 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cut_exit(t_env **env, t_env **exp, t_shell *shell, t_cmd *cmd)
{
	char		*tmp;
	long long	valeur;
	char		*line;
	char		**split;

	line = rejoinstr(cmd->arg);
	split = ft_supersplit(line, ' ');
	valeur = ft_mega_atoi(line);
	tmp = ft_itoa(valeur);
	if (split[1] != NULL)
	{
		ft_exit_error_1(split, line, tmp, 0);
		return ;
	}
	else if (ft_strcmp(line, tmp))
		ft_error_exit_2(split, tmp, line);
	else
	{
		free(line);
		free_all(env, exp, shell);
		ft_exit_2(split, valeur, tmp);
	}
	free(line);
	free_tab(split);
}
