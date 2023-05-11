/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:43:24 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 15:58:12 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cut_hd_cmd(t_cmd *cmd)
{
	char	*str;

	str = readline("<");
	if (g_rvalue == 130)
	{
		if (str)
			free(str);
		return (0);
	}
	if (!str)
		return (0);
	if (ft_strcmp(cmd->limiter, str) == 0)
	{
		free(str);
		return (0);
	}
	ft_putstr_fd(str, cmd->fd_in);
	free(str);
	return (1);
}

void	heredoc_cmd(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell)
{
	int		frk;
	int		value;

	value = 0;
	cmd->fd_in = create_fd_hd(cmd);
	frk = fork();
	if (frk == 0)
	{
		signal(SIGINT, handler_heredoc);
		while (1)
		{
			if (cut_hd_cmd(cmd) == 0)
				break ;
		}
		free_all(env, exp, shell);
		exit(0);
	}
	else
	{
		waitpid(frk, &value, 0);
		if (WEXITSTATUS(value) == 2)
			g_rvalue = 130;
	}
}

int	cut_hd_no_cmd(char *limiter)
{
	char	*str;

	str = readline("<");
	if (g_rvalue == 130)
	{
		if (str)
			free(str);
		return (0);
	}
	if (!str)
		return (0);
	if (ft_strcmp(limiter, str) == 0)
	{
		free(str);
		return (0);
	}
	free(str);
	return (1);
}

void	heredoc_nocmd(char *limiter, t_env **env, t_env **exp, t_shell *shell)
{
	int		frk;
	int		value;

	value = 0;
	frk = fork();
	if (frk == 0)
	{
		signal(SIGINT, handler_heredoc);
		while (1)
		{
			if (cut_hd_no_cmd(limiter) == 0)
				break ;
		}
		free_all(env, exp, shell);
		exit(0);
	}
	else
	{
		waitpid(frk, &value, 0);
		if (WEXITSTATUS(value) == 2)
			g_rvalue = 130;
	}
}

void	handler_heredoc(int sig)
{
	if (sig == 2)
	{
		free_all(recup_struct_env2(NULL, 2), recup_struct_env2(NULL, 6),
			recup_shell(NULL));
		g_rvalue = 130;
		printf("\n");
		exit(2);
	}
}
