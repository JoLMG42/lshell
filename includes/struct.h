/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:23:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/10 00:10:03 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

# define STRUCT_H

typedef struct s_syntax
{
	int			sq_opened;
	int			dq_opened;
	int			braces;
	int			in_quote;
	int			i;
	int			j;
}	t_s;

typedef struct s_cmd
{
	char			*cmd;
	int				pos;
	int				fd_in;
	int				fd_out;
	char			*name_in;
	char			*name_out;
	int				mode_open;
	int				is_hd;
	char			*limiter;
	int				bracelvl;
	char			**subshell;
	char			**arg;
	int				pid;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_tree
{
	char			*ope;
	t_cmd			*cmd_left;
	t_cmd			*cmd_right;
	int				in_exec;
	struct s_tree	*next;
}	t_tree;

typedef struct s_env
{
	char			*line;
	char			*name;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	int		pid;
	int		pipefd[2];
	int		tmpfd;
	int		skip;
	int		first_to_wait;
	char	*saveope;
	t_tree	*tree;
	t_env	*env;
	t_env	*exp;
}	t_shell;

extern int	g_rvalue;

#endif
