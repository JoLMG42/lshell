/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:26:08 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 15:45:15 by jtaravel         ###   ########.fr       */
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
int     pars_prompt(char *str, t_env *env, t_env *exp, int mode);
void	ft_exit(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell);
char    *rejoinstr(char **tab);
int     ft_isalpha(char c);
char	**dup_tab(char **tab);
char *parse_string(char *str, t_env **env);
int     check_brace(char *str);
int     check_parse_sq_dq(char *str, char c);
void    cd_no_arg(t_env **env, t_env **exp);
char    *recup_content_env(char *find, t_env **env);
void    create_oldpwd(t_env **env, int mode);
void    update_pwd_oldpwd(t_env **env, char *newpath, int mode, t_env **exp);
char    *add_spaces(char *str, int i, int j);
char	*add_space_malloc(char *str);
char	*cut_add_space_while3(char *res, char *str, int i, int j);
char	*cut_add_space_while2(char *res, char *str, int i, int j);
char	*cut_add_space_while(char *res, char *str, int i, int j);
int	add_space_quote(int in_quote, char *str, int i);
int	check_syntax(char **tab);
char	**rejointab(char **tab);
void	parsefirstcmd(t_tree **lst, t_env **env, char *str);
void	parsearg(t_tree **lst, t_env **env);
char    *reparse_dol(char *str, t_env **env);;
int     check_parse_sq_dq(char *str, char c);;
void	parseargfirstcmd(t_tree **lst, t_env **env);
void	parsearg(t_tree **lst, t_env **env);
void	parsecmd(t_tree **lst, t_env **env, char *str);
void	setwildcardsfirstcmd(t_tree **lst);
void	setwildcards(t_tree **lst);
void	setbracelvl(t_tree **lst);
void	setbracelvlfirstcmd(t_tree **lst);
void	setarg(t_tree **lst);
void	setargfirstcmd(t_tree **lst);
void	ouverturefirstcmd(t_tree **lst);
void	ouvertureredir(t_tree **lst);
char    *reparse_dol(char *str, t_env **env);
void	ft_putchar_fd(char c, int fd);
char	*ft_suppr_dq_sq(char *str);
void	ft_suppr(char **tab, t_env **env);
int	ft_check_options(char **tab);
void	putstr_fd_echo(char *str, int fd);
char	*cut_sds(char *str, t_s *s, int i, int c);
void    free_all(t_env **env, t_env **exp, t_shell *shell);
void    cut_exit(t_env **env, t_env **exp, t_shell *shell, t_cmd *cmd);
int     ft_mega_atoi(char *str);
void    ft_exit_error_1(char **split, char *line, char *tmp, int mode);
void    ft_error_exit_2(char **split, char *tmp);
void     ft_exit_2(char **split, int valeur, char *tmp);
void    export_both(char *str, t_env **env, t_env **exp);
t_shell *recup_shell(t_shell *shell);
t_tree  *dup_tree(t_tree **adup);
char    *recalculcmd(char *cmd, char *str, char *ope);
char    *addspacedol(char *str);
t_tree  *cut_parseargfirstcmd_2(t_tree *tmp, char **tab, t_s *s, char *str);
t_tree  *cut_parseargfirstcmd_3(t_tree *tmp, char **tab, t_s *s, char *str);
t_tree  *cut_parseargfirstcmd(t_tree *tmp, char **tab, t_s *s, char *str);
t_env   *recup_struct_env(t_env **env, int mode);
t_tree  *cut_parsfirstcmd_3(t_tree *tmp, t_s *s, char **recup);
t_tree  *cut_parsfirstcmd_2(t_tree *tmp, t_s *s, char **tab, char *str);
t_tree  *cut_parsfirstcmd(t_tree *tmp, t_s *s, char **tab, char *str);
t_tree  *cut_parsecmd_2(t_tree *tmp, t_s *s, char **tab, char *str);
t_tree  *cut_parsecmd_env(t_tree *tmp, t_s *s, char **tab, t_env *env);
t_tree  *cut_parsecmd(t_tree *tmp, t_s *s, char **tab, char *str);
void    free_stuff(t_env **env, t_env **exp);
int     check_dot(char *str);
void    ft_wait(t_cmd **cmd);
void    ft_wait_all(t_tree *to_wait, t_tree *end, int first);
void    last_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp);
t_env   **recup_struct_env2(t_env **env, int mode);
void    middle_execute(t_cmd **cmd, t_env **env, t_shell *tree, int fd_temp);
void    cut_middle_execute_free(char **et, char **ext);
void    first_execute(t_cmd **cmd, t_env **env, t_shell *tree, t_env **exp);
void    executeone(t_cmd **cmd, t_env **env, t_shell *shell, t_env **exp);
int     lst_tree_size(t_tree **tree);
void    exec_no_next(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell);
void    exec_one_only(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell);
t_tree  *exec_pp(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell);
t_tree  *exec_ee(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell);
t_tree  *exec_ou(t_tree *tmp, t_tree *to_wait, t_env **exp, t_shell *shell);
t_tree  *exec_sec_pp_end(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
t_tree  *exec_sec_grp_pp_2(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
void    exec_sec_grp_pp(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell);
t_tree  *exec_sec_grp_ee(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
t_tree  *ex_sec_grp_ee(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
t_tree  *little_cut(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
t_tree  *exec_sec_grp_ou(t_tree *tmp, t_tree *tw, t_env **exp, t_shell *shell);
void    init_hd(t_tree *tmp, t_env **env, t_env **exp, t_shell *shell);
t_tree  *skip_pipe(t_tree *tmp, t_shell *shell);
void    init_heredoc(t_tree **tree, t_env **env, t_env **exp, t_shell *shell);
void    handler_heredoc(int sig);
void    heredoc_nocmd(char *limiter, t_env **env, t_env **exp, t_shell *shell);
int     cut_hd_no_cmd(char *limiter);
void    heredoc_cmd(t_cmd *cmd, t_env **env, t_env **exp, t_shell *shell);
int     create_fd_hd(t_cmd *cmd);


#endif
