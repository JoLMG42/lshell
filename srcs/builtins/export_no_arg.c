/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:30:37 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/08 17:41:53 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_sort_ascii(char **av)
{
	int		i;
	int		j;
	int		len;
	char	*swap;

	len = tab_len(av);
	i = 0;
	while (i < len)
	{
		j = 0;
		while (j < len)
		{
			if (ft_strcmp(av[i], av[j]) < 0)
			{
				swap = av[i];
				av[i] = av[j];
				av[j] = swap;
			}
			j++;
		}
		i++;
	}
	return (av);
}

void	export_no_arg(t_env **exp)
{
	t_env	*tmp;
	t_env	*idx;
	t_env	*tmpnull;
	char	**recup;

	recup = list_to_tab(exp);
	recup = ft_sort_ascii(recup);
	get_env(&tmp, recup, 0, &tmpnull);
	idx = tmp->next;
	while (idx)
	{
		if (!check_egal(idx->line))
			printf("export %s\n", idx->line);
		else
			printf("export %s=\"%s\"\n", idx->name, idx->content);
		idx = idx->next;
	}
	ft_lstclear_env(&tmp, del);
	ft_lstclear_env(&tmpnull, del);
	free_tab(recup);
}
