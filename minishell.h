/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:26:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/11 11:27:32 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

#define MINISHELL_H

#include "struct.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <linux/random.h>


void    check_sq_dq(t_s *s_s, char c);
void    init_syntax_struct(t_s *s);
char    **ft_supersplit(char *s, char c);
char    *ft_strjoin(char *s1, char *s2);
int	ft_strlen(char *str);
char    *ft_strdup(char *s);

#endif
