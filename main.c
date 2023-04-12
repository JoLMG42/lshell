/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/12 20:08:29 by jtaravel         ###   ########.fr       */
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


void    init_syntax_struct(t_s *s)
{
        s->dq_opened = 0;
        s->sq_opened = 0;
	s->braces = 0;
}

void    check_sq_dq(t_s *s_s, char c)
{
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
	if (c == '(' && s_s->braces == 0)
	{
		s_s->braces = 1;
	}
	else if (c == ')' && s_s->braces == 1)
		s_s->braces = 0;
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
			c++;
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>')
			c++;
		i++;
	}
	res = malloc(sizeof(char *) * (ft_strlen(str) + c + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
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
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
		}
		if (str[i] == '(' || str[i] == ')')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			res[j] = ' ';
			j++;
			i++;
		}
		if (str[i] == '"' && str[i - 1] == '=')
		{
			dq = 1;
			space = 1;
		}
		if (str[i] == '"'  && dq == 0 && sq == 0 && str[i - 1] != '=')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			i++;
			dq = 1;
		}
		if (str[i] == '\''  && sq == 0 && dq == 0 && str[i - 1] != '=')
		{
			res[j] = ' ';
			j++;
			res[j] = str[i];
			j++;
			i++;
			sq = 1;
		}
		if (str[i] == '"'  && dq == 1 && space == 0)
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
		}
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
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
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

t_tree	*fonctiontestpoudoublepipes(t_tree **lst)
{
	t_tree	*tmp;
	char	**tab;
	int	i;

	tmp = *lst;
	while (tmp)
	{
		if (tmp->cmd_left)
		{
			tab = ft_split(tmp->cmd_left, "||");
			i = 0;
			while (tab[i])
			{
				printf("tab[i]: %s\n", tab[i]);
				if (i == 0)
					ft_lstadd_backtree(lst, ft_lstnewtree(ft_strdup("||"), NULL, ft_strdup(tab[i])));
				else
					ft_lstadd_backtree(lst, ft_lstnewtree(ft_strdup("||"), ft_strdup(tab[i - 1]), ft_strdup(tab[i])));
				i++;
				i++;
			}
		}
		tmp = tmp->next;
			
	}
	return *lst;
}

char	**rejointab(char **tab)
{
	int	i;
	int	j;
	int	h;
	char	**res;

	i = 0;
	j = 0;
	h = 1;
	res = malloc(1000000);
	res[j] = ft_strdup(tab[0]);
	while (tab[h])
	{
		if (ft_strcmp(tab[h], "|") == 0 || ft_strcmp(tab[h], "&&") == 0 || ft_strcmp(tab[h], "||") == 0)
		{
			j++;
			res[j] = ft_strjoin(res[j], tab[h]);
			j++;
		}
		else
		{
			res[j] = ft_strjoin(res[j], tab[h]);
			res[j] = ft_strjoin(res[j], " ");
		}
		h++;
	}
	res[j] = 0;
	return res;
}

int	pars_prompt(char *str)
{
	char	*recup;
	char	**tab;
	t_cmd	*l_cmd;
	t_tree	*tree;
	char	**suppr;

	suppr = malloc(sizeof(char *) * ft_strlen(str) + 1);
	l_cmd = malloc(sizeof(struct s_cmd));
	tree = malloc(sizeof(struct s_tree));
	printf("recup prompt: %s\n", str);
	recup = add_spaces(str);
	printf("AFTER ADD SPACES: %s\n", recup);
	tab = ft_split(recup, " ");
//	if (!check_syntax(tab))
//		return (0);
	int	i = 0;
	int	c = 0;
	while (tab[i])
	{
		printf("after SPLIT: %s\n", tab[i]);
		i++;
	}
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
	}
	i = 1;
	while (tab[i])
	{
		ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("&&"), ft_strdup(tab[i - 1]), ft_strdup(tab[i])));
		//tree = tree->next;

		//printf("\n\n\n\nTAB[i]: %s\n\n\n", tab[i]);
		i++;
		//printf("\n\n\n\nTAB[i]: %s\n\n\n", tab[i]);
	}*/
	tab = rejointab(tab);
	i = 1;
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "&&") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("&&"), ft_strdup(tab[i - 1]), ft_strdup(tab[i + 1])));
		}
		if (ft_strcmp(tab[i], "||") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("||"), ft_strdup(tab[i - 1]), ft_strdup(tab[i + 1])));
		}
		if (ft_strcmp(tab[i], "|") == 0)
		{
			ft_lstadd_backtree(&tree, ft_lstnewtree(ft_strdup("|"), ft_strdup(tab[i - 1]), ft_strdup(tab[i + 1])));
		}
		i++;

	}
	t_tree	*test = tree->next;//fonctiontestpoudoublepipes(&tree->next);
	while (test && i > 1)
	{
		printf("OPERATOR: %s\n", test->ope);
		printf("CMD LEFT: %s\n", test->cmd_left);
		printf("CMD right: %s\n", test->cmd_right);
		test = test->next;
	}
	/*i = 0;
	while (tab[i])
	{
		char **tmp = ft_split(tab[i], "||");
		int j = 0;
		while (tmp[j] && tmp[1])
		{
			suppr[c] = ft_strdup(tab[i]);
			c++;
			printf("tab[i] si || -> %s\n\n\n", tab[i]);	
			ft_lstadd_back(&l_cmd, ft_lstnew(ft_strdup(tmp[j]), LeftDP));
			l_cmd->next->prev = l_cmd;
			l_cmd = l_cmd->next;
			j++;
		}
		free_tab(tmp);
		i++;
	}
	printf("\n\n\n");
	while (l_cmd->prev)
	{
		l_cmd = l_cmd->prev;
	}
	l_cmd = l_cmd->next;
	t_cmd	*list = l_cmd;
	while (list->next)
	{
		c = 0;
		while (suppr[c])
		{
			if (ft_strcmp(suppr[c], list->cmd) == 0)
				ft_lstdelone(list, del);
			c++;
		}
		list = list->next;
	}
	l_cmd = list;
	while (l_cmd->prev)
	{
		if (ft_strlen(l_cmd->cmd) > 0)
			printf("Commande -> %s     POS-> %d\n", l_cmd->cmd, l_cmd->pos);
		l_cmd = l_cmd->prev;
	}
	//printf("test: %s\n", l_cmd->cmd);
	printf("\n\n\n");
	//printf("ON REJOIN TOUT APRES PARSING\n\n\n");
	//char	*res = resjoin(tab);
	//printf("%s\n\n\n", res);*/
	return (1);
}

int	main(int ac, char **av, char **env)
{
	char	*str;

	while (1)
	{
		str = readline("ft_containers$ ");
		if (!str)
			break ;
		if (!pars_prompt(str))
			printf("INVALID SYNTAX\n");
		if (str[0])
			add_history(str);
	}

}
