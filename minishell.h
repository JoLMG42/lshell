/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:26:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/27 00:36:23 by jtaravel         ###   ########.fr       */
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
# include <dirent.h>


void    check_sq_dq(t_s *s_s, char c);
void    init_syntax_struct(t_s *s);
char    **ft_supersplit(char *s, char c);
char    *ft_strjoin(char *s1, char *s2);
char    *ft_strjoin2(char *s1, char *s2);
int	ft_strlen(char *str);
char    *ft_strdup(char *s);
char	**ft_split(char *str, char *charset);
t_cmd   *ft_lstnew(char *cmd);
void    ft_lstdelone(t_cmd *lst, void (*del)(void *));
void    ft_lstclear(t_cmd **lst, void (*del)(void *));
void    ft_lstadd_back(t_cmd **alst, t_cmd *new);
void    del(void *data);
t_cmd   *ft_lstlast(t_cmd *lst);
t_tree  *ft_lstnewtree(char *ope, t_cmd *cmdl, t_cmd *cmdr);
void    ft_lstdelonetree(t_tree *lst, void (*del)(void *));
void    ft_lstcleartree(t_tree **lst, void (*del)(void *));
t_tree  *ft_lstlasttree(t_tree *lst);
void    ft_lstadd_backtree(t_tree **alst, t_tree *new);
char    **ft_supersplit2(char *s, char c);
void	free_tab(char **tab);
void    setwildcardsfirstcmd(t_tree **lst);
int	tab_len(char **tab);
int	ft_strcmp(char *s1, char *s2);
int	ft_strncmp(char *s1, char *s2, size_t len);
void    setwildcards(t_tree **lst);
void get_env(t_env **env, char **envi, int mode, t_env **exp);
t_env   *ft_lstnew_env(char *line, char *name, char *content);
void    ft_lstclear_env(t_env **lst, void (*del)(void *));
char    **list_to_tab(t_env **env);
int     listenv_size(t_env **env);
void    exec(t_tree **tree, t_env **env, t_env **exp, t_shell *shell);
void    heredoc(t_cmd **cmd, t_env **env, t_env **exp, t_shell *shell);
void	handler(int sig);
t_tree  *recup_struct(t_tree **tree, int mode);
char    *ft_itoa(int n);
void    handler_fork(int sig);
void	ft_putstr_fd(char *str, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_echo(t_cmd *cmd, char **tab, t_env **env);
int     check_builtins(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell);
void	ft_env(t_cmd *cmd, t_env **env);
void    ft_lstdelone_env(t_env *lst, void (*del)(void *));
void	ft_unset(char **tab, t_env **env, t_env **exp);
int     is_builtins(char *cmd);
void    putstr_fd_echo(char *str, int fd);
void    ft_cd(char **tab, t_env **env, t_env **exp);
void    ft_lstadd_back_env(t_env **alst, t_env *new);
t_env   *ft_lstlast_env(t_env *lst);
void    ft_export(char **tab, t_env **env, t_env **exp);
char    *ft_suppr_dq_sq(char *str);
char    *before_egal(char *str);
int     len_egal(char *str);
int     check_name(char *str);
void    ft_pwd(t_cmd *cmd);
t_env   *var_in_exp(char *str, t_env **exp);
char    *reparse_dol(char *str, t_env **env);
void    exec_and(t_cmd **cmd, t_env **env, t_env **exp, t_shell *shell);
void	ft_wait(t_cmd **cmd);
char	*recup_cmd(char *cmd, t_env **env, int i);
char	**recover_path(char **envp);
char	*check_slash(char *cmd, int mode);
char	**fusioncmdarg(char *cmd, char **arg);
void	del_sq_dq_arg(char **tab);
int     pars_prompt(char *str, t_env *env, t_env *exp);
void	ft_exit(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell);
char    *rejoinstr(char **tab);
int     ft_isalpha(char c);
char	**dup_tab(char **tab);


#endif
