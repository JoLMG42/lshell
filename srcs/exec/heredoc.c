/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 14:10:05 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 15:55:42 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

int	create_fd_hd(t_cmd *cmd)
{
	char	random[11];
	char	*name;
	int		dev;
	int		fd;

	dev = open("/dev/urandom", O_RDONLY);
	read(dev, random, 10);
	random[10] = 0;
	name = ft_strdup(".tmp_file");
	if (cmd->name_in)
		free(cmd->name_in);
	cmd->name_in = ft_strjoin(name, random);
	fd = open(cmd->name_in, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		printf("ERROR FD HEREDOC\n");
		return (0);
	}
	return (fd);
}

void	heredoc(t_cmd **cmd, t_env **env, t_env **exp, t_shell *shell)
{
	t_cmd	*tmp;
	int		tmpvalue;

	tmpvalue = g_rvalue;
	g_rvalue = 0;
	tmp = (*cmd);
	if (!tmp->cmd && !tmp->limiter)
		return ;
	if (!tmp->cmd)
	{
		heredoc_nocmd(tmp->limiter, env, exp, shell);
		if (g_rvalue == 0)
			g_rvalue = tmpvalue;
		return ;
	}
	heredoc_cmd(tmp, env, exp, shell);
}
