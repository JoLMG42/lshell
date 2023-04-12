/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:23:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/12 18:21:59 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

# define STRUCT_H

# define LeftDP 6
# define RightDP 7
# define LeftET 8
# define RightET 9

typedef struct s_syntax
{
	int			sq_opened;
	int			dq_opened;
	int			braces;
}	t_s;

typedef	struct	s_cmd
{
	char	*cmd;
	int	pos;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef	struct	s_tree
{
	char	*ope;
	char	*cmd_left;
	char	*cmd_right;
	struct	s_tree	*next;
}	t_tree;

#endif
