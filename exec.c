/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 13:21:12 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/03 18:31:50 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_fork(int sig)
{
	if (sig == 2)
	{
		g_rvalue = 130;
		printf("\n");
		return ;
	}
	if (sig == 3)
	{
		g_rvalue = 131;
		printf("Quit (core dumped)\n");
		return ;
	}
}

void	init_heredoc(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;

	tmp = (*tree)->next;
	if (tmp && tmp->cmd_left->is_hd)
		heredoc(&tmp->cmd_left, env, exp, shell);
	while (tmp)
	{
		if (tmp->cmd_right->is_hd)
			heredoc(&tmp->cmd_right, env, exp, shell);
		tmp = tmp->next;
	}
}

void	init_hd(t_tree *tmp, t_env **env, t_env **exp,t_shell *shell)
{
	if (tmp && tmp->cmd_left && tmp->cmd_left->is_hd)
		heredoc(&tmp->cmd_left, env, exp, shell);
	if (tmp && tmp->cmd_right && tmp->cmd_right->is_hd)
		heredoc(&tmp->cmd_right, env, exp, shell);

	
}

void	exec(t_tree **tree, t_env **env, t_env **exp, t_shell *shell)
{
	t_tree	*tmp;
	int	i;
	int	tmpfd;
	int	pipefd[2];
	t_tree	*to_wait;
	int	first_to_wait;
	t_tree *toto;

	i = 0;
	shell->saveope = NULL;
	init_heredoc(tree, env, exp, shell);
	tmp = (*tree)->next;
	pipe(shell->pipefd);
	(*tree)->next->in_exec = 1;
	signal(SIGINT, handler_fork);
	signal(SIGQUIT, handler_fork);
	//signal(SIGPIPE, SIG_IGN);
	int	flag = 0;
	char	*ttt;
	tmpfd = 0;
	to_wait = NULL;
	first_to_wait = 1;
	while (tmp)
	{
		if (tmp->cmd_right->cmd == NULL)
		{
			if (tmp->cmd_left->bracelvl)
				pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
			else
			{
				executeone(&tmp->cmd_left, env, shell, exp);
				ft_wait(&(tmp->cmd_left));
			}
			if (tmp->cmd_left->is_hd)
			{
				if (tmp->cmd_left->name_in)
					unlink(tmp->cmd_left->name_in);
			}
			break ;
		}
		if (i == 0)
		{

			if (!tmp->next)
			{
				if (ft_strcmp(tmp->ope, "|") == 0)
				{
					if (tmp->cmd_left->bracelvl)
					{
						pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
						break ;
					}
					first_execute(&tmp->cmd_left, env, shell, exp);
					if (tmp->cmd_right->bracelvl)
					{
						ttt = ft_strdup(tmp->cmd_right->cmd);
						pars_prompt(ttt, *env, *exp, 2);
						free(ttt);
						break ;
					}
					last_execute(&tmp->cmd_right, env, shell, exp);
					ft_wait(&(tmp->cmd_left));
					ft_wait(&(tmp->cmd_right));
				}
				else if (ft_strcmp(tmp->ope, "&&") == 0)
				{
					if (tmp->cmd_left->bracelvl)
					{
						pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
					}
					else
					{
						exec_and(&tmp->cmd_left, env, exp, shell);
						ft_wait(&(tmp->cmd_left));
					}
					if (g_rvalue == 0)
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							ft_wait(&(tmp->cmd_right));
						}
					}
				}
				else if (ft_strcmp(tmp->ope, "||") == 0)
				{
					if (tmp->cmd_left->bracelvl)
					{
						pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
					}
					else
					{
						exec_and(&tmp->cmd_left, env, exp, shell);
						ft_wait(&(tmp->cmd_left));
					}
					if (g_rvalue != 0)
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							ft_wait(&(tmp->cmd_right));
						}
					}
				}
			}
			else if (ft_strcmp(tmp->ope, "|") == 0) //premier groupe + pipe + next
			{
				if (ft_strcmp(tmp->next->ope, "|") == 0)
				{
					if (tmp->cmd_left->bracelvl)
					{
						pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
					}
					else
					{
						first_execute(&tmp->cmd_left, env, shell, exp);
						//ft_wait(&(tmp->cmd_left));
						tmpfd = shell->pipefd[0];
						pipe(shell->pipefd);
					}
					if (tmp->cmd_right->bracelvl)
					{
						pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
					//	continue ;
					}
					else
					{
						middle_execute(&tmp->cmd_right, env, shell, tmpfd);
						//ft_wait(&(tmp->cmd_right));
						to_wait = tmp;
					}
				}
				else
				{
					if (tmp->cmd_left->bracelvl)
					{
						pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
						//break ;
					}
					else
						first_execute(&tmp->cmd_left, env, shell, exp);
					if (tmp->cmd_right->bracelvl)
					{
						pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
					}
					else
					{
						last_execute(&tmp->cmd_right, env, shell, exp);
					}
					if (!tmp->cmd_left->bracelvl)
						ft_wait(&(tmp->cmd_left));
					if (!tmp->cmd_right->bracelvl)
						ft_wait(&(tmp->cmd_right));
				}
			}
			else if (ft_strcmp(tmp->ope, "&&") == 0) //premier groupe + et + next
			{
				//printf("PREMIER GROUPE\n");
				if (tmp->cmd_left->bracelvl)
				{
					pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
				}
				else
				{
					exec_and(&tmp->cmd_left, env, exp, shell);
					ft_wait(&(tmp->cmd_left));
				}
				if (g_rvalue == 0)
				{
					if (ft_strcmp(tmp->next->ope, "|") == 0)
					{

						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							first_execute(&tmp->cmd_right, env, shell, exp);
							to_wait = tmp;
							first_to_wait = 0;
							//ft_wait(&(tmp->cmd_right));
						}
					}
					else
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							ft_wait(&(tmp->cmd_right));
						}
					}
				}
				else
				{
					while (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
						//printf("AUT SKIP LO\n");
						tmp = tmp->next;
					}
				}
			}
			else if (ft_strcmp(tmp->ope, "||") == 0) //premier groupe + ou + next
			{
				if (tmp->cmd_left->bracelvl)
				{
					pars_prompt(tmp->cmd_left->cmd, *env, *exp, 2);
				}
				else
				{
					exec_and(&tmp->cmd_left, env, exp, shell);
					ft_wait(&(tmp->cmd_left));
				}
				if (g_rvalue != 0)
				{
					if (ft_strcmp(tmp->next->ope, "|") == 0)
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
				//			printf("OUIOUOI OU PUIS PIPE \n");
							first_execute(&tmp->cmd_right, env, shell, exp);
							to_wait = tmp;
							first_to_wait = 0;
						}
					}
					else
					{
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							ft_wait(&(tmp->cmd_right));
						}
					}
				}
				else
				{
					while (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
						//printf("AUT SKIP LO\n");
						tmp = tmp->next;
					}
				}
			}
		} // deuxieme groupe
		else
		{
			if (tmp->next && ft_strcmp(shell->saveope, "|") == 0 && ft_strcmp(tmp->ope, "|") == 0
				&& ft_strcmp(tmp->next->ope, "|") == 0)
			{
	//			printf("CMD 1 = %s\n", tmp->cmd_right->cmd);
	//			printf("CMD iciii = %s\n", tmp->cmd_right->cmd);
				tmpfd = shell->pipefd[0];
				pipe(shell->pipefd);
				if (tmp->cmd_right->bracelvl)
				{
					pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
				}
				else
					middle_execute(&tmp->cmd_right, env, shell, tmpfd);
				//ft_wait(&(tmp->cmd_right));
			}
			else if (ft_strcmp(shell->saveope, "|") == 0 && ft_strcmp(tmp->ope, "|") == 0) // fin de la serie de pipe
			{
	//			printf("CMD 2 = %s\n", tmp->cmd_right->cmd);
	//			printf("FIN DE PIPE 1\n");
				if (tmp->cmd_right->bracelvl)
				{
					pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
				}
				else
				{
					last_execute(&tmp->cmd_right, env, shell, exp);
					//ft_wait(&(tmp->cmd_right));
					ft_wait_all(to_wait, tmp, first_to_wait);
					to_wait = NULL;
				}
			}
			else if (ft_strcmp(tmp->ope, "&&") == 0)
			{
				if (g_rvalue == 0)
				{
					if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
	//					printf("CMD 5 = %s\n", tmp->cmd_right->cmd);
						pipe(shell->pipefd);
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							first_execute(&tmp->cmd_right, env, shell, exp);
							to_wait = tmp;
							first_to_wait = 0;
						}
					}
					else
					{
						printf("CMD 3 = %s\n", tmp->cmd_right->cmd);
						if (tmp->cmd_right->bracelvl)
						{
						//	toto = dup_tree(&tmp);
						//	recup_struct(&toto, 9);
							pars_prompt(ft_strdup(tmp->cmd_right->cmd), *env, *exp, 2);
						//	tmp = toto;
						//	flag = 998;
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							ft_wait(&(tmp->cmd_right));
						}
						//last_execute(&tmp->cmd_right, env, shell, exp);
					}
				}
				else
				{
					while (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
						//printf("AUT SKIP LO\n");
						tmp = tmp->next;
					}
				}
			}
			else if (ft_strcmp(tmp->ope, "||") == 0)
			{
				if (g_rvalue != 0)
				{
					if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
		//				printf("CMD 5 = %s\n", tmp->cmd_right->cmd);
						pipe(shell->pipefd);
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							first_execute(&tmp->cmd_right, env, shell, exp);
							to_wait = tmp;
							first_to_wait = 0;
						}
					}
					else
					{
		//				printf("CMD 3 = %s\n", tmp->cmd_right->cmd);
						if (tmp->cmd_right->bracelvl)
						{
							pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
						}
						else
						{
							exec_and(&tmp->cmd_right, env, exp, shell);
							//last_execute(&tmp->cmd_right, env, shell, exp);
							ft_wait(&(tmp->cmd_right));
						}
					}
				}
				else
				{
					while (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
					{
						//printf("AUT SKIP LO\n");
						tmp = tmp->next;
					}
				}
			}
			else if (ft_strcmp(tmp->ope, "|") == 0)
			{
				if (tmp->next && ft_strcmp(tmp->next->ope, "|") == 0)
				{
		//			printf("ON CONTINUE LES PIPES\n");
					tmpfd = shell->pipefd[0];
					pipe(shell->pipefd);
					if (tmp->cmd_right->bracelvl)
					{
						pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
					}
					else
						middle_execute(&tmp->cmd_right, env, shell, tmpfd);
				}
				else
				{
		//			printf("FIN DE PIPE 2\n");
					//exec_and(&tmp->cmd_right, env, exp, shell);
					if (tmp->cmd_right->bracelvl)
					{
						pars_prompt(tmp->cmd_right->cmd, *env, *exp, 2);
					}
					else
					{
						last_execute(&tmp->cmd_right, env, shell, exp);
						if (!to_wait)
							to_wait = tmp;
						ft_wait_all(to_wait, tmp, first_to_wait);
						to_wait = NULL;
					}
				}
			}
		i++;
		if (tmp && tmp->cmd_left->is_hd)
		{
			if (tmp->cmd_left->name_in)
				unlink(tmp->cmd_left->name_in);
		}
		if (tmp && tmp->cmd_right->is_hd)
		{
			if (tmp->cmd_right->name_in)
				unlink(tmp->cmd_right->name_in);
		}
		if (shell->saveope)
		{
			free(shell->saveope);
			shell->saveope = NULL;
		}
		shell->saveope = ft_strdup(tmp->ope);
		tmp = tmp->next;
	}
	if (shell && shell->saveope)
	{
		free(shell->saveope);
		shell->saveope = NULL;
	}
}
