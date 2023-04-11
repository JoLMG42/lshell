/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 14:56:26 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/11 17:07:28 by jtaravel         ###   ########.fr       */
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
			printf("res = %s\n", res);
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

int	pars_prompt(char *str)
{
	char	*recup;
	char	**tab;
	printf("recup prompt: %s\n", str);
	recup = add_spaces(str);
	printf("AFTER ADD SPACES: %s\n", recup);
	tab = ft_supersplit(recup, ' ');
	if (!check_syntax(tab))
		return (0);
	int	i = 0;
	while (tab[i])
	{
		printf("after SPLIT: %s\n", tab[i]);
		i++;
	}
	//printf("ON REJOIN TOUT APRES PARSING\n\n\n");
	//char	*res = resjoin(tab);
	//printf("%s\n\n\n", res);
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
