/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 10:48:27 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/26 14:43:22 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	*str;

	str = getcwd(NULL, 0);
	ft_putstr_fd(str, cmd->fd_out);
	free(str);
}
