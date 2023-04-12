/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:26:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/12 18:32:11 by jtaravel         ###   ########.fr       */
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
char	**ft_split(char *str, char *charset);
t_cmd   *ft_lstnew(char *cmd, int pos);
void    ft_lstdelone(t_cmd *lst, void (*del)(void *));
void    ft_lstclear(t_cmd **lst, void (*del)(void *));
void    ft_lstadd_back(t_cmd **alst, t_cmd *new);
void    del(void *data);
t_cmd   *ft_lstlast(t_cmd *lst);
t_tree  *ft_lstnewtree(char *ope, char *cmdl, char *cmdr);
void    ft_lstdelonetree(t_tree *lst, void (*del)(void *));
void    ft_lstcleartree(t_tree **lst, void (*del)(void *));
t_tree  *ft_lstlasttree(t_tree *lst);
void    ft_lstadd_backtree(t_tree **alst, t_tree *new);

#endif
