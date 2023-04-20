/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 14:10:05 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/20 12:13:14 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	handler_heredoc(int sig);

void	heredoc_nocmd(char *limiter)
{
	char	*str;
	int	frk;
	int	value;

	value = 0;
	frk = fork();
	if (frk == 0)
	{
		signal(SIGINT, handler_heredoc);
		while (1)
		{
			str = readline("<");
			if (g_rvalue == 130)
			{
				if (str)
					free(str);
				break ;
			}
			if (!str)
				break ;
			if (ft_strcmp(limiter, str) == 0)
			{
				free(str);
				break ;
			}
			free(str);
		}
		exit(0);
	}
	else
	{
		waitpid(frk, &value, 0);
	}
}

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
	int	dev;
	int	fd;

	dev = open("/dev/urandom", O_RDONLY);
	read(dev, random, 10);
	random[10] = 0;
	name = ft_strdup(".tmp_file");
	cmd->name_in = ft_strjoin(name, random);
	fd = open(cmd->name_in, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		printf("ERROR FD HEREDOC\n");
		return (0);
	}
	return fd;

}

void	heredoc_cmd(t_cmd *cmd)
{
	char	*str;
	int	frk;
	int	value;

	value = 0;
	cmd->fd_in = create_fd_hd(cmd);
	frk = fork();
	if (frk == 0)
	{
		while (1)
		{
			signal(SIGINT, handler_heredoc);
			str = readline("<");
			if (g_rvalue == 130)
			{
				if (str)
					free(str);
				break ;
			}
			if (!str)
				break ;
			if (ft_strcmp(cmd->limiter, str) == 0)
			{
				free(str);
				break ;
			}
			ft_putstr_fd(str, cmd->fd_in);
			free(str);
		}
		exit(0);
	}
	else
	{
		waitpid(frk, &value, 0);
	}
}

void	handler_heredoc(int sig)
{
	if (sig == 2)
	{
		g_rvalue = 130;
		printf("\n");
		exit(2);
	}
}

void	heredoc(t_cmd **cmd)
{
	t_cmd	*tmp;
	int	tmpvalue;
	
	tmpvalue = g_rvalue;
	g_rvalue = 0;
	tmp = (*cmd);
	if (!tmp->cmd)
	{
		heredoc_nocmd(tmp->limiter);
		if (g_rvalue == 0)
			g_rvalue = tmpvalue;
		return ;
	}
	heredoc_cmd(tmp);
	if (g_rvalue == 0)
		g_rvalue = tmpvalue;
}
