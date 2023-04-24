/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/24 15:55:42 by jtaravel         ###   ########.fr       */
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
}

void    check_sq_dq(t_s *s_s, char c)
{
	int	cop;

	cop = 0;
        if (c == '\'' && s_s->dq_opened == 0)
        {
                if (s_s->sq_opened == 0)
                        s_s->sq_opened = 1;
                else
                        s_s->sq_opened = 0;
        }
        if (c == '"' && s_s->sq_opened == 0)
        {
                if (s_s->dq_opened == 0)
                        s_s->dq_opened = 1;
                else
                        s_s->dq_opened = 0;
        }
	if (c == '(')
	{
		cop++;
		s_s->braces++;
	}
	if (c == ')' && s_s->braces)
	{
		cop--;
		s_s->braces--;
	}
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*add_spaces(char *str)
{
	int i;
	int j;
	int c;
	int	dq;
	int	sq;
	int	space;
	char *res;

	i = 0;
	c = 0;
	dq = 0;
	sq = 0;
	space = 0;
	while (str[i])
	{
		if ((str[i] == '|' && str[i + 1] == '|') || (str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i + 1] == '>'))
		{
			c +=2;
			i++;
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '(' || str[i] == ')')
			c += 1;
		i++;
	}
	res = malloc(sizeof(char *) * (ft_strlen(str) + c + 1));
	i = 0;
	j = 0;
	while (str && str[i])
	{
		/*if (str[i] == '(' || str[i] == ')')
		{
			res[j] = str[i];
			j++;
			i++;
		}*/
		if ((str[i] == '|' && str[i + 1] == '|') || (str[i] == '&' && str[i + 1] == '&')
			|| (str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i + 1] == '>'))
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			i++;
			j++;
			res[j] = str[i];
			i++;
			j++;
			res[j] = ' ';
			j++;
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
		}
		else if (str[i] == '(' || str[i] == ')')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
		}
/*		else if (str[i] == '"' && str[i - 1] == '=')
		{
			dq = 1;
			space = 1;
			res[j] = str[i];
			i++;
			j++;
		}
		else if (str[i] == '"' && dq == 1 && space == 1)
		{
			res[j] = str[i];
			j++;
			i++;
		}
		else if (str[i] == '"' && dq == 0 && sq == 0 && str[i - 1] != '=')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			i++;
			dq = 1;
		}
		else if (str[i] == '\''  && sq == 0 && dq == 0 && str[i - 1] != '=')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			i++;
			sq = 1;
		}
		else if (str[i] == '"'  && dq == 1 && space == 0)
		{
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
			dq = 0;
		}
		else if (str[i] == '\''  && sq == 1 && space == 0)
		{
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
			sq = 0;
		}*/
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
		space = 0;
	}
	res[j] = 0;
	return (res);

}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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
		if (tab[i][0] == '\'')
		{
			j = 1;
			sq = 1;
			while (tab[i][j])
			{
				if (tab[i][j] == '\'')
					sq = 0;
				j++;
			}
		}
		if (tab[i][0] == '"')
		{
			dq = 1;
			j = 1;
			while (tab[i][j])
			{
				if (tab[i][j] == '"')
					dq = 0;
				j++;
			}
		}
		i++;
	}
	if (dq == 1 || sq == 1)
		return (0);
	return (1);
}

char	*resjoin(char **tab)
{
	int		i;
	char	*res;

	i = 1;
	res = ft_strdup(tab[0]);;
	while (tab[i])
	{
		res = ft_strjoin(res, tab[i]);
		i++;
	}
	return res;
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

	if (ft_strcmp(ope, "<") == 0)
	{
		tab = ft_supersplit(cmd, ' ');
		i = 0;
		printf("TA[i] = %s\n\n\n\n", tab[i]);
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
			if (ft_strcmp(tab[i], str) && ft_strcmp(tab[i], ope))
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
			tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
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
			free(tmp->cmd_left->cmd);
			tmp->cmd_left->cmd = ft_strdup(tab[i - 1]);
			tmp->cmd_left->name_out = ft_strdup(tab[i + 1]);
			tmp->cmd_left->mode_open = 2;
		}
		if (ft_strcmp(tab[i], "<<") == 0)
		{
			if (tab[i + 2])
				tmp->cmd_left->cmd = ft_strdup(tab[i + 2]);
			else
				tmp->cmd_left->cmd = NULL;
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

int	len_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			break;
		i++;
	}
	return (i);
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
					tmp->cmd_right->cmd = ft_strdup(tab[i - 1]);
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
					tmp->cmd_right->name_out = ft_strdup(tab[i + 1]);
					tmp->cmd_right->mode_open = 2;
				}
				if (ft_strcmp(tab[i], "<<") == 0)
				{
					if (tab[i + 2])
					{
						free(tmp->cmd_right->cmd);
						tmp->cmd_right->cmd = ft_strdup(tab[i + 2]);
					}
					else
						tmp->cmd_right->cmd = NULL;
					tmp->cmd_right->limiter = ft_strdup(tab[i + 1]);
					tmp->cmd_right->is_hd = 1;
					tmp->cmd_right->name_in = ft_strdup(tab[i + 1]);
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

int	tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	setbracelvl(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;
	int	j;
	int	count;

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
		count = 0;
		while (tab && tab[i])
		{
			if (ft_strcmp(tab[i], "(") == 0)
			{
				j = i + 1;
				free(tmp->cmd_right->cmd);
				tmp->cmd_right->cmd = NULL;
				tmp->cmd_right->bracelvl++;
				count++;
				while (tab[j])
				{
					if (ft_strcmp(tab[j], "(") == 0)
						count--;
					if (ft_strcmp(tab[j], ")") == 0)
						count--;
					else
					{
						tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, tab[j]);
							tmp->cmd_right->cmd = ft_strjoin(tmp->cmd_right->cmd, " ");
					}
					j++;
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
	if (!tmp)
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
	free_tab(tab);
}

char	*addspacedol(char *str)
{
	int	i;
	int	j;
	int	c;
	char	*res;
	t_s	s_s;

	init_syntax_struct(&s_s);
	i = 0;
	c = 0;
	while (str[i])
	{
		if (str[i] == '$')
			c += 2;
		else
			c++;
		i++;
	}
	res = malloc(sizeof(char *) * (c + 10));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' && s_s.sq_opened == 0 && s_s.dq_opened == 1)
		{
			ft_suppr_dq_sq(str);
			ft_suppr_dq_sq(res);
		}
		check_sq_dq(&s_s, str[i]);
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
	//free(str);
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

	tmp = (*lst)->next;
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
				if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
				{
				//	int test = recuppid(env);
					tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], "999");
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
				tmp->cmd_left->arg[i] = ft_strjoin(tmp->cmd_left->arg[i], tab[j]);
			j++;
		}
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
						tmp->cmd_right->arg[i] = ft_strjoin(tmp->cmd_right->arg[i], "999");
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

	tmp = (*lst)->next;
	if (!tmp || !tmp->cmd_left->cmd)
		return ;
	tmp->cmd_left->cmd = addspacedol(tmp->cmd_left->cmd);
	tab = ft_supersplit(tmp->cmd_left->cmd, ' ');
	free(tmp->cmd_left->cmd);
	tmp->cmd_left->cmd = NULL;
	j = 0;
	while (tab && tab[j])
	{
		//tab[j] = reparse_dol(tab[j], env);
		if (ft_strcmp(tab[j], "$") == 0)
		{
			if (tab[j + 1] && ft_strcmp(tab[j + 1], "$") == 0)
			{
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, "999");
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
			if (!tab[j + 1])
				tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[j]);
			j++;
		}
		else
			tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, tab[j]);
		tmp->cmd_left->cmd = ft_strjoin(tmp->cmd_left->cmd, " ");
		j++;
	}
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
		tmp = tmp->next;
	}
}
	
int	pars_prompt(char *str, t_env *env, t_env *exp)
{
	char	*recup;
	char	**tab;
	t_cmd	*l_cmd;
	t_tree	*tree;
	t_shell	*shell;

	shell = malloc(sizeof(struct s_shell));
	shell->env = NULL;
	shell->exp = NULL;
	shell->tree = NULL;
	//env = malloc(sizeof(struct s_env));
	//l_cmd = malloc(sizeof(struct s_cmd));
	//tree = malloc(sizeof(struct s_tree));
//	printf("recup prompt: %s\n", str);
	recup = add_spaces(str);
//	printf("AFTER ADD SPACES: %s\n", recup);
	//tab = ft_split(recup, " ");
	tab = NULL;
	tab = ft_supersplit(recup, ' ');
	free(recup);
	if (!tab[0])
	{
		free_tab(tab);
		free(shell);
		return (1);
	}
	if (!check_syntax(tab))
	{
		free(shell);
		return (0);
	}
	/*int j = 0;
	while (tab[j])
	{
		printf("YYYYYYYYYYYYYY %s\n", tab[j]);
		j++;
	}*/
	int	i = 0;
	int	c = 0;
	/*tab = ft_split(recup, "&&");
	i = 0;
	while (tab[i])
	{
		ft_lstadd_back(&l_cmd, ft_lstnew(ft_strdup(tab[i]), LeftET));
		if (i == 0)
			l_cmd->prev = NULL;
		l_cmd->next->prev = l_cmd;
		l_cmd = l_cmd->next;
		i++;
	}*/
	tab = rejointab(tab);
//	while (tab[i])
//	{
///		printf("after REJOIN: %s\n", tab[i]);
//		i++;
//	}
	i = 1;
	tree = ft_lstnewtree(NULL, NULL, NULL);
	if (!tab[1])
	{
		ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
			ft_lstnew(ft_strdup(tab[0])), ft_lstnew(NULL)));

	}
	while (tab && tab[i])
	{
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("&&"),
				ft_lstnew(ft_strdup(tab[i - 1])), ft_lstnew(ft_strdup(tab[i + 1]))));
			i += 2;
		}
		else if (ft_strcmp(tab[i], "||") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("||"),
				ft_lstnew(ft_strdup(tab[i - 1])), ft_lstnew(ft_strdup(tab[i + 1]))));
			i += 2;
		}
		else if (ft_strcmp(tab[i], "|") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("|"),
				ft_lstnew(ft_strdup(tab[i - 1])), ft_lstnew(ft_strdup(tab[i + 1]))));
			i += 2;
		}
		else
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(NULL,
				ft_lstnew(ft_strdup(tab[i])), ft_lstnew(NULL)));
			i++;
		}
	}
	free_tab(tab);
	t_env	*tmp_env = env;
	t_tree	*test = tree->next;
	parsefirstcmd(&tree, &env);
	setargfirstcmd(&tree);
	ouverturefirstcmd(&tree);
	setbracelvlfirstcmd(&tree);
	setwildcardsfirstcmd(&tree);
	parseargfirstcmd(&tree, &env);
	if (i > 1)
	{
		parsecmd(&tree, &env);
		setarg(&tree);
		ouvertureredir(&tree);
		setbracelvl(&tree);
		setwildcards(&tree);
		parsearg(&tree, &env);
	}
	while (test)
	{
		printf("			OPERATOR: %s\n", test->ope);
		printf("CMD LEFT: %s\n", test->cmd_left->cmd);
		int n = 0;
		if (test->cmd_left->arg)
		{
			while (test->cmd_left->arg[n])
			{
				printf("ARG A LA CMD: %s\n", test->cmd_left->arg[n]);
				n++;
			}
		}
		printf("REDIR IN: %s\nREDIR OUT: %s\nIS_HD: %d\nLIMITER: %s\nBRACE LVL: %d\n\n\n\n", test->cmd_left->name_in, test->cmd_left->name_out, test->cmd_left->is_hd, test->cmd_left->limiter, test->cmd_left->bracelvl);
		printf("CMD RIGHT: %s\n", test->cmd_right->cmd);
		n = 0;
		if (test->cmd_right->arg)
		{
			while (test->cmd_right->arg[n])
			{
				printf("ARG A LA CMD: %s\n", test->cmd_right->arg[n]);
				n++;
			}
		}
		printf("REDIR IN: %s\nREDIR OUT: %s\nIS_HD: %d\nLIMITER: %s\nBRACE LVL: %d\n\n\n\n", test->cmd_right->name_in, test->cmd_right->name_out, test->cmd_right->is_hd, test->cmd_right->limiter, test->cmd_right->bracelvl);
		test = test->next;
	}
	recup_struct(&tree);
	shell->tree = tree;
	shell->env = env;
	shell->exp = exp;
	exec(&tree, &env, &exp, shell);


	ft_lstcleartree(&tree, del);
	//ft_lstclear_env(&env, del);
	free(shell);
	return (1);
}

t_tree	*recup_struct(t_tree **tree)
{
	static t_tree *tmp;
	if (tree != NULL)
		tmp = *tree;
	if (tree == NULL && tmp)
		return (tmp->next);
	return NULL;
}

void	handler(int sig)
{
	t_tree *tmp = recup_struct(NULL);
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
	ft_lstclear_env(env, del);
	ft_lstclear_env(exp, del);
}

int	main(int ac, char **av, char **envp)
{
	char	*str;
	t_shell	*shell;
	t_env	*env;
	t_env	*exp;
	
	get_env(&env, envp);
	get_env(&exp, envp);
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("ft_containers$ ");
		if (!str)
			break ;
		if (!pars_prompt(str, env, exp))
			printf("INVALID SYNTAX\n");
		if (str[0])
			add_history(str);
		free(str);
	}
	free_stuff(&env, &exp);
}
