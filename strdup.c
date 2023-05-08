/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 11:26:47 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 21:53:40 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *s)
{
	char	*cpy;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	cpy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!cpy)
		return (0);
	while (s[i])
	{
		cpy[i] = s[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

void	del(void *data)
{
	free(data);
}

char	*little_cut_parse_first(char **tab, t_s *s)
{
	tab[s->j] = ft_suppr_dq_sq(tab[s->j]);
	if (recup_content_env(tab[s->j] + 1,
			recup_struct_env2(NULL, 2)) != NULL)
		tab[s->j] = recup_content_env(tab[s->j] + 1,
				recup_struct_env2(NULL, 2));
	return (tab[s->j]);
}

void	cut_e_one_f_2(t_cmd *tmp, t_shell *s, char **envtab, char **et)
{
	if (tmp->cmd)
		check_slash(tmp->cmd, 1);
	free_all(recup_struct_env2(NULL, 2),
		recup_struct_env2(NULL, 6), s);
	cut_middle_execute_free(envtab, et);
	exit(127);
}

void	braces_parsing(t_shell **shell)
{
	t_shell	*recup;
	t_tree	*cpy;

	recup = recup_shell(NULL);
	*shell = recup;
	cpy = recup_struct(NULL, 1);
	ft_lstcleartree(&cpy, del);
}
