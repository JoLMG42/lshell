/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/01 23:27:32 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include "struct.h"
#include "minishell.h"

int	g_rvalue = 0;


void    init_syntax_struct(t_s *s)
{
        s->dq_opened = 0;
        s->sq_opened = 0;
	s->braces = 0;
	s->in_quote = 0;
	s->i = 0;
	s->j = 0;
}

int	check_parse_sq_dq(char *str, char c)
{
	int	i;
	int	count;
	int	opo;
	int	in_opo;
	int	in_actu;

	opo = '\'';
	if (c == '\'')
		opo = '\"';
	i = 0;
	count = 0;
	in_opo = 0;
	in_actu = 0;
	while (str[i])
	{
		if (str[i] == opo && !in_opo && !in_actu)
			in_opo = 1;
		else if (str[i] == opo && in_opo)
			in_opo = 0;
		if (str[i] == c && !in_opo && !in_actu)
		{
			count++;
			in_actu = 1;

		}
		else if (str[i] == c && in_actu)
		{
			count++;
			in_actu = 0;
		}
		i++;
	}
	if (count % 2 != 0)
		return (0);
	return (1);
}

int	check_brace(char *str)
{
	int	i;
	int	in_brace;

	i = 0;
	in_brace = 0;
	while (str[i])
	{
		if (str[i] == '(')
			in_brace++;
		if (str[i] == ')')
			in_brace--;
		i++;
	}
	if (in_brace != 0)
		return (0);
	return (1);
}

int	check_syntax(char **tab)
{
	int	i;
	int	j;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (tab[i])
	{
		if ((ft_strcmp(tab[i], "||") == 0 || ft_strcmp(tab[i], "|") == 0 || ft_strcmp(tab[i], "<") == 0
			|| ft_strcmp(tab[i], "&&") == 0 || ft_strcmp(tab[i], ">") == 0 || ft_strcmp(tab[i], "<<") == 0
			|| ft_strcmp(tab[i], ">>") == 0) && !tab[i + 1])
			return (0);
		if (ft_strcmp(tab[i], "||") == 0)
		{
			if (ft_strcmp(tab[i + 1], "|") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "|") == 0)
		{
			if (ft_strcmp(tab[i + 1], "||") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "|") == 0)
		{
			if (ft_strcmp(tab[i + 1], "&&") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			if (ft_strcmp(tab[i + 1], "|") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "||") == 0)
		{
			if (ft_strcmp(tab[i + 1], "&&") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			if (ft_strcmp(tab[i + 1], "||") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "|") == 0)
		{
			if (ft_strcmp(tab[i + 1], "|") == 0)
				return (0);
		}
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			if (ft_strcmp(tab[i + 1], "&&") == 0)
				return (0);
		}
		if (!check_brace(tab[i]))
			return (0);
		if (!check_parse_sq_dq(tab[i], '\''))
			return (0);
		if (!check_parse_sq_dq(tab[i], '\"'))
			return (0);
		i++;
	}
	if (dq == 1 || sq == 1)
		return (0);
	return (1);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}
int	ft_lstsize(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	**rejointab(char **tab)
{
	int	i;
	int	j;
	int	h;
	char	**res;

	i = 0;
	j = 0;
	h = 0;
	res = malloc(sizeof(char *) * (10000));
	//res[0] = ft_strdup(tab[0]);
	//j++;
	while (tab[h])
	{
		if (tab[h + 1] && ft_strcmp(tab[h], "|") == 0 || ft_strcmp(tab[h], "&&") == 0 || ft_strcmp(tab[h], "||") == 0)
			//|| ft_strcmp(tab[h], "<") == 0 || ft_strcmp(tab[h], "<<") == 0 || ft_strcmp(tab[h], ">") == 0
			//|| ft_strcmp(tab[h], ">>") == 0)
		{
			j++;
			res[j] = NULL;
			res[j] = ft_strjoin(res[j], tab[h]);
			if (tab[h + 1] && ft_strcmp(tab[h + 1], "|") == 0 || ft_strcmp(tab[h + 1], "&&") == 0 || ft_strcmp(tab[h + 1], "||") == 0)
				//|| ft_strcmp(tab[h + 1], "<") == 0 || ft_strcmp(tab[h + 1], "<<") == 0 || ft_strcmp(tab[h + 1], ">") == 0
				//|| ft_strcmp(tab[h + 1], ">>") == 0)
				;
			else
			{
				j++;
				res[j] = NULL;
			}
		}
		else
		{
			if (j == 0 && h == 0)
			{
				res[j] = NULL;
				res[j] = ft_strdup(tab[h]);
				if (tab[h + 1])
					res[j] = ft_strjoin(res[j], " ");

			}
			else
			{
				res[j] = ft_strjoin(res[j], tab[h]);
				res[j] = ft_strjoin(res[j], " ");
			}
		}
		h++;
	}
	free_tab(tab);
	res[j + 1] = 0;
	/*i = 0;
	while (res[i])
	{
		printf("LUNA LPB = %s\n\n\n\n\n", res[i]);
		i++;
	}*/
	return res;
}

int	len_space(char *str);

char	*recalculcmd(char *cmd, char *str, char *ope)
{
	char	**tab;
	char	*res;
	int	i;

	res = NULL;
	if (ft_strcmp(ope, "<") == 0)
	{
		tab = ft_supersplit(cmd, ' ');
		i = 0;
		while (tab[i])
		{
			if (str && ft_strcmp(tab[i], str) && ft_strcmp(tab[i], ope))
				res = ft_strdup(tab[i]);
			i++;
		}
		free_tab(tab);
	}
	if (ft_strcmp(ope, "<<") == 0)
	{
		tab = ft_supersplit(cmd, ' ');
		i = 0;
		while (tab[i])
		{
			if (str && ft_strcmp(tab[i], str) && ft_strcmp(tab[i], ope))
				res = ft_strdup(tab[i]);
			i++;
		}
		free_tab(tab);
	}
	free(cmd);
	return (res);

}

void	ouverturefirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left || !tmp->cmd_left->cmd)
		return ;
	i = 0;
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	while (tab[i])
	{
		if (ft_strcmp(tab[i], ">") == 0)
		{
			free(tmp->cmd_left->cmd);
			if (i > 0)
				tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
			else
				tmp->cmd_left->cmd = NULL;
			tmp->cmd_left->name_out = ft_strdup(tab[i + 1]);
			tmp->cmd_left->mode_open = 1;
		}
		if (ft_strcmp(tab[i], "<") == 0)
		{
			tmp->cmd_left->name_in = ft_strdup(tab[i + 1]);
			tmp->cmd_left->mode_open = 1;
			tmp->cmd_left->cmd = recalculcmd(tmp->cmd_left->cmd, tmp->cmd_left->name_in, "<");
		}
		if (ft_strcmp(tab[i], ">>") == 0)
		{
			if (i > 0)
			{
				free(tmp->cmd_left->cmd);
				tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
			}
			if (i == 0)
			{
				free(tmp->cmd_left->cmd);
				tmp->cmd_left->cmd = NULL;
			}
			tmp->cmd_left->name_out = ft_strdup(tab[i + 1]);
			tmp->cmd_left->mode_open = 2;
		}
		if (ft_strcmp(tab[i], "<<") == 0)
		{
			if (tab[i + 1] && tab[i + 2])
			{
				if (tmp->cmd_left->cmd)
					free(tmp->cmd_left->cmd);
				tmp->cmd_left->cmd = ft_strdup(tab[i + 2]);
			}
			else
			{
				if (tmp->cmd_left->cmd)
					free(tmp->cmd_left->cmd);
				tmp->cmd_left->cmd = NULL;
			}
			tmp->cmd_left->limiter = ft_strdup(tab[i + 1]);
			tmp->cmd_left->is_hd = 1;
			tmp->cmd_left->name_in = ft_strdup(tab[i + 1]);
			if (tmp->cmd_left->cmd)
				tmp->cmd_left->cmd = recalculcmd(tmp->cmd_left->cmd, tmp->cmd_left->name_in, "<<");
		}
		i++;
	}
	free_tab(tab);
}
void	ouvertureredir(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	i = 0;
	while (tmp)
	{
		if (tmp->cmd_right && tmp->cmd_right->cmd)
		{
			tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
			i = 0;
			while (tab && tab[i])
			{
				if (ft_strcmp(tab[i], ">") == 0)
				{
					free(tmp->cmd_right->cmd);
					if (i > 0)
						tmp->cmd_right->cmd = ft_strdup(tab[i - 1]);
					else
						tmp->cmd_right->cmd = NULL;
					tmp->cmd_right->name_out = ft_strdup(tab[i + 1]);
					tmp->cmd_right->mode_open = 1;
				}
				if (ft_strcmp(tab[i], "<") == 0)
				{
					tmp->cmd_right->name_in = ft_strdup(tab[i + 1]);
					tmp->cmd_right->mode_open = 1;
				}
				if (ft_strcmp(tab[i], ">>") == 0)
				{
					if (i > 0)
					{
						free(tmp->cmd_right->cmd);
						tmp->cmd_right->cmd = ft_strdup(tab[i - 1]);
					}
					if (i == 0)
					{
						free(tmp->cmd_right->cmd);
						tmp->cmd_right->cmd = NULL;
					}
					tmp->cmd_right->name_out = ft_strdup(tab[i + 1]);
					tmp->cmd_right->mode_open = 2;
				}
				if (ft_strcmp(tab[i], "<<") == 0)
				{
					if (tab[i + 1] && tab[i + 2])
					{
						free(tmp->cmd_right->cmd);
						tmp->cmd_right->cmd = ft_strdup(tab[i + 2]);
					}
					else
					{
						if (tmp->cmd_right->cmd)
							free(tmp->cmd_right->cmd);
						tmp->cmd_right->cmd = NULL;
					}
					tmp->cmd_right->limiter = ft_strdup(tab[i + 1]);
					tmp->cmd_right->is_hd = 1;
					//tmp->cmd_right->name_in = ft_strdup(tab[i + 1]);
					tmp->cmd_right->name_in = NULL;
					if (tmp->cmd_right->cmd)
						tmp->cmd_right->cmd = recalculcmd(tmp->cmd_right->cmd, tmp->cmd_right->name_in, "<<");
				}
				i++;
			}
			free_tab(tab);
		}
		tmp = tmp->next;
	}
}

void	setbracelvl(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	j;
	int	in_brace;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	tab = NULL;
	while (tmp)
	{
		if (!tmp->cmd_right->cmd)
			return ;
		tab = ft_supersplit2(tmp->cmd_right->cmd, ' ');
		i = 0;
		in_brace = 0;
		while (tab && tab[i])
		{
			if (ft_strcmp(tab[i], "(") == 0)
			{
				j = i + 1;
				free(tmp->cmd_right->cmd);
				tmp->cmd_right->cmd = NULL;
				tmp->cmd_right->bracelvl = 1;
				in_brace++;
				while (tab[j] && in_brace)
				{
					if (ft_strcmp(tab[j], "(") == 0)
						in_brace++;
					if (ft_strcmp(tab[j], ")") == 0)
						in_brace--;
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[j]);
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
					j++;
					if (ft_strcmp(tab[j], ")") == 0 && in_brace == 1)
						break ;
					i++;
				}
	
			}
			i++;
		}
		free_tab(tab);
		tmp = tmp->next;
	}
}

void	setbracelvlfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	j;
	int	count;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	if (!tmp->cmd_left->cmd)
		return ;
	tab = ft_supersplit2(tmp->cmd_left->cmd, ' ');
	i = 0;
	count = 0;
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "(") == 0)
		{
			j = i + 1;
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = NULL;
			tmp->cmd_left->bracelvl++;
			count++;
			while (tab[j])
			{
				if (ft_strcmp(tab[j], "(") == 0)
				{
				//	tmp->cmd_left->bracelvl++;
					count--;
				}
				if (ft_strcmp(tab[j], ")") == 0)
				{
					//tmp->cmd_left->bracelvl++;
					count--;
				}
				else
				{
					tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[j]);
					tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				}
				j++;
			}

		}
		i++;
	}
	free_tab(tab);
}

void	setarg(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	j;
	int	n;

	tmp = (*lst)->next;
	if (!tmp)
		return ;
	while (tmp)
	{
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		tmp->cmd_right->arg = malloc(sizeof(char *) * (tab_len(tab) + 1));
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = ft_strdup(tab[0]);
		i = 1;
		j = 0;
		while (tab[i])
		{
			if (tab[i][0] == '>')
				break ;
			if (tab[i][0] == '<' || tab[0][0] == '<')
			{
				free(tmp->cmd_right->cmd);
				tmp->cmd_right->cmd = NULL;
				break ;
			}
			tmp->cmd_right->arg[j] = ft_strdup(tab[i]);
			j++;
			i++;
		}
		n = 1;
		while (tab[n])
		{
			if (tab[n][0] == '>')
			{
				while (tab[n])
				{
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[n]);
					n++;
				}
				break ;
			}
			if (tab[0][0] == '<' || tab[n][0] == '<')
			{
				n = 0;
				while (tab[n])
				{
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[n]);
					n++;
				}
				break ;
			}
			n++;
		}
		tmp->cmd_right->arg[j] = 0;
		tmp = tmp->next;
		free_tab(tab);
	}
}

void	setargfirstcmd(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	j;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	tmp->cmd_left->arg = malloc(sizeof(char *) * (tab_len(tab) + 1));
	i = 1;
	j = 0;
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = ft_strdup(tab[0]);
	while (tab[i])
	{
		if (tab[i][0] == '>')
			break ;
		if (tab[i][0] == '<' || tab[0][0] == '<')
		{
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = NULL;
			break ;
		}
		tmp->cmd_left->arg[j] = ft_strdup(tab[i]);
		j++;
		i++;
	}
	i = 1;
	while (tab[i])
	{
		if (tab[i][0] == '>')
		{
			while (tab[i])
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[i]);
				i++;
			}
			break ;
		}
		if (tab[0][0] == '<' || tab[i][0] == '<')
		{
			i = 0;
			while (tab[i])
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[i]);
				i++;
			}
			break ;
		}
		i++;
	}
	tmp->cmd_left->arg[j] = 0;
	/*i = 0;
	while (tmp->cmd_left->arg[i])
	{
		printf("JJJJJJ = %s\n", tmp->cmd_left->arg[i]);
		i++;
	}*/
	free_tab(tab);
}

char	*addspacedol(char *str)
{
	int	i;
	int	j;
	int	c;
	int	f;
	char	*res;
	t_s	s_s;

	init_syntax_struct(&s_s);
	i = 0;
	c = 0;
	f = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			c += 2;
			f = 1;
		}
		else
			c++;
		i++;
	}
	if (f == 0)
		return (str);
	res = malloc(sizeof(char *) * (c + 10));
	i = 0;
	j = 0;
	while (str[i])
	{
		check_sq_dq(&s_s, str[i]);
		/*if (str[i] == '"' && s_s.sq_opened == 0 && s_s.dq_opened == 1)
		{
			ft_suppr_dq_sq(str);
			ft_suppr_dq_sq(res);
		}*/
		if (str[i] == '$' && s_s.sq_opened == 0 && s_s.dq_opened == 0)
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			i++;
			j++;
			res[j] = ' ';
			j++;
		}
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	free(str);
	res[j] = 0;
	return (res);
}

int	listenv_size(t_env **env)
{
	t_env	*tmp;
	int	i;

	i = 0;
	tmp = (*env)->next;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);

}

char	**list_to_tab(t_env **env)
{
	t_env	*tmp;
	int	i;
	char	**res;

	tmp = (*env)->next;
	res = malloc(sizeof(char *) * (listenv_size(env) + 1));
	i = 0;
	while (tmp)
	{
		res[i] = ft_strdup(tmp->line);
		i++;
		tmp = tmp->next;
	}
	res[i] = 0;
	return (res);
}

int	recuppid(t_env **env)
{
	char	**envtab;

	envtab = list_to_tab(env);
	int i = 0;
	while (envtab[i])
	{
		i++;
	}
	//int	fd = open("psa", O_CREAT | O_RDONLY, 0644);
	char *cmd[2];
	cmd[0] = "/bin/ps";
	cmd[1] = "a";
	if (execve(cmd[0], cmd, envtab) == -1)
		printf("lol\n");
	return (0);
}

void	parseargfirstcmd(t_tree **lst, t_env **env)
{
	t_tree	*tmp;
	t_env	*tmpenv;
	int	i;
	int	j;
	char	**tab;
	char	*str;
	int	flag = 0;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->arg)
		return ;
	i = 0;
	while (tmp->cmd_left->arg[i])
	{
		tmp->cmd_left->arg[i] = addspacedol(tmp->cmd_left->arg[i]);
		tab = ft_supersplit(tmp->cmd_left->arg[i], ' ');
		free(tmp->cmd_left->arg[i]);
		tmp->cmd_left->arg[i] = NULL;
		j = 0;
		while (tab[j])
		{
			if (ft_strcmp(tab[j], "$") == 0)
			{
				flag = 1;
				if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
				{
				//	int test = recuppid(env);
					tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], "$$");
				}
				else if (tab[j + 1] && ft_strcmp(tab[j + 1], "?") == 0)
				{
					str = ft_itoa(g_rvalue);
					tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], str);
					free(str);
				}
				else if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") != 0)
				{
					tmpenv = (*env)->next;
					while (tmpenv)
					{
						if (ft_strcmp(tmpenv->name, tab[j + 1]) == 0)
							tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], tmpenv->content);
						tmpenv = tmpenv->next;
					}
				}
				if (!tab[j + 1])
					tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], tab[j]);
				j++;
					
			}
			else
			{
				tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], tab[j]);
			}
			if (tab[j + 1])// && tab[j][0] != '$' && flag == 0)
			{
				tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], " ");
			}
			j++;
		}
	flag = 0;
	free_tab(tab);
	i++;
	}
}

void	parsearg(t_tree **lst, t_env **env)
{
	t_tree	*tmp;
	t_env	*tmpenv;
	int	i;
	int	j;
	char	**tab;
	char	*str;

	tmp = (*lst)->next;
	while (tmp)
	{
		i = 0;
		while (tmp->cmd_right && tmp->cmd_right->arg[i])
		{
			tmp->cmd_right->arg[i] = addspacedol(tmp->cmd_right->arg[i]);
			tab = ft_supersplit(tmp->cmd_right->arg[i], ' ');
			free(tmp->cmd_right->arg[i]);
			tmp->cmd_right->arg[i] = NULL;
			j = 0;
			while (tab && tab[j])
			{
				if (ft_strcmp(tab[j], "$") == 0)
				{
					if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
					{
					//	int test = recuppid(env);
						tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], "$$");
					}
					else if (tab[j + 1] && ft_strcmp(tab[j + 1], "?") == 0)
					{
						str = ft_itoa(g_rvalue);
						tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], str);
						free(str);
					}
					else if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") != 0)
					{
						tmpenv = (*env)->next;
						while (tmpenv)
						{
							if (ft_strcmp(tmpenv->name, tab[j + 1]) == 0)
								tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], tmpenv->content);
							tmpenv = tmpenv->next;
						}
					}
					if (!tab[j + 1])
						tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], tab[j]);
					j++;
				}
				else
					tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], tab[j]);

				j++;
			}
			free_tab(tab);
			i++;
		}
		tmp = tmp->next;
	}
}

void	parsefirstcmd(t_tree **lst, t_env **env)
{
	t_env	*tmpenv;
	t_tree	*tmp;
	char	**tab;
	int	j;
	char	*str;
	int	flag = 0;
	char	**recup;

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	recup = dup_tab(tab + 1);
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = ft_strdup(tab[0]);
	tmp->cmd_left->cmd = addspacedol(tmp->cmd_left->cmd);
	free_tab(tab);
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = NULL;
	j = 0;
	while (tab && tab[j])
	{
		//printf("TAB[j] = %s\n", tab[j]);
		//tab[j] = reparse_dol(tab[j], env);
		if (ft_strcmp(tab[j], "$") == 0)
		{
			flag = 1;
			if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, "$");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, "$");
			}
			else if (tab[j + 1] && ft_strcmp(tab[j + 1], "?") == 0)
			{
				str = ft_itoa(g_rvalue);
				if (j > 1)
					tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, str);
				free(str);
			}
			else if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") != 0)
			{
				tmpenv = (*env)->next;
				while (tmpenv)
				{
					if (tmpenv->name && ft_strcmp(tmpenv->name, tab[j + 1]) == 0)
						tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tmpenv->content);
					tmpenv = tmpenv->next;
				}
			}
			else
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[j]);
			j++;
			if (!tab[j] || !tab[j + 1])
				break ;
			//if (tab[j] == NULL)
			//	break ;
		}
		else
		{
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[j]);
		}
		if (tab[j] && tab[j][0] != '$' && flag == 0)
		{
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
		}
		j++;
		flag = 0;
	}
	j = 0;
	while (recup[j])
	{
		//if (recup[j + 1])
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, recup[j]);
		j++;
		if (recup[j])
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
	}
	free_tab(recup);
	free_tab(tab);
}

void	parsecmd(t_tree **lst, t_env **env)
{
	t_env	*tmpenv;
	t_tree	*tmp;
	char	**tab;
	int	j;
	char	*str;

	tmp = (*lst)->next;
	while (tmp)
	{
		tmp->cmd_right->cmd = addspacedol(tmp->cmd_right->cmd);
		tab = ft_supersplit(tmp->cmd_right->cmd, ' ');
		free(tmp->cmd_right->cmd);
		tmp->cmd_right->cmd = NULL;
		j = 0;
		while (tab && tab[j])
		{
			if (ft_strcmp(tab[j], "$") == 0)
			{
				if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
				{
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, "999");
				}
				else if (tab[j + 1] && ft_strcmp(tab[j + 1], "?") == 0)
				{
					str = ft_itoa(g_rvalue);
					if (j > 1)
						tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, str);
					free(str);
				}
				else if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") != 0)
				{
					tmpenv = (*env)->next;
					while (tmpenv)
					{
						if (ft_strcmp(tmpenv->name, tab[j + 1]) == 0)
							tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tmpenv->content);
						tmpenv = tmpenv->next;
					}
				}
				if (!tab[j + 1])
					tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[j]);
				j++;
			}
			else
				tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[j]);
			tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
			j++;
		}
		free_tab(tab);
		tmp = tmp->next;
	}
}

t_tree	*recup_struct(t_tree **tree, int mode)
{
	static t_tree *tmp;
	if (mode == 3)
		tmp = NULL;
	if (tree != NULL)
		tmp = *tree;
	if (tree == NULL && tmp && mode == 2)
		return (tmp->next);
	if (tree == NULL && tmp && mode == 1)
		return (tmp);
	return NULL;
}

void	handler(int sig)
{
	t_tree *tmp = recup_struct(NULL, 2);
	if (tmp && tmp->in_exec)
	{
		//printf("\n");
		return ;
	}
	else if (sig == 2 && tmp && !tmp->in_exec)
	{
		rl_on_new_line();
		printf("\n");
		rl_replace_line("", 1);
		rl_redisplay();
		g_rvalue = 130;
		return ;
	}
	else if (!tmp && sig == 2)
	{
		rl_on_new_line();
		printf("\n");
		rl_replace_line("", 1);
		rl_redisplay();
		g_rvalue = 130;
		return ;
	}


}

void	free_stuff(t_env **env, t_env **exp)
{
	t_tree	*tmp;
	t_shell	*shell;

	tmp = recup_struct(NULL, 1);
	shell = recup_shell(NULL);
	free(shell);
	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
	ft_lstcleartree(&tmp, del);
}

t_tree	*dup_tree(t_tree **adup)
{
	t_tree	*tmp;
	t_tree	*ret;

	tmp = (*adup);
	ret = ft_lstnewtree(NULL, NULL, NULL);
	while (tmp)
	{
		ft_lstadd_backtree(&ret, ft_lstnewtree(ft_strdup(tmp->ope), ft_lstnew(tmp->cmd_left->cmd), ft_lstnew(tmp->cmd_right->cmd)));
		tmp = tmp->next;
	}
	return (ret->next);
}

int	main(int ac, char **av, char **envp)
{
	char	*str;
	t_shell	*shell;
	t_env	*env;
	t_env	*exp;
	
	get_env(&env, envp, 0, &exp);
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("ft_containers$ ");
		if (!str)
			break ;
		if (!pars_prompt(str, env, exp, 5))
			printf("INVALID SYNTAX\n");
		if (str[0])
			add_history(str);
		free(str);
	}
	free_stuff(&env, &exp);
}
