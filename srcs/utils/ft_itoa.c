/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <jtaravel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 00:16:20 by jtaravel          #+#    #+#             */
/*   Updated: 2023/05/04 16:48:08 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_fill(char *s, int *tab, int i, int n)
{
	int	j;

	j = 0;
	if (n >= 0)
	{
		while (i != 0)
		{
			s[j++] = tab[i] + 48;
			i--;
		}
		s[j] = tab[i] + 48;
	}
	else
	{
		s[j] = '-';
		j++;
		while (i != 1)
		{
			s[j++] = tab[i] + 48;
			i--;
		}
		s[j] = tab[i] + 48;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	int		i;
	long	nb;
	char	*res;
	int		tab[255];

	i = 0;
	nb = n;
	if (nb < 0)
	{
		i++;
		nb = nb * -1;
	}
	while (nb > 9)
	{
		tab[i] = nb % 10;
		i++;
		nb = nb / 10;
	}
	tab[i] = nb;
	res = malloc(sizeof(char) * (i + 2));
	if (!res)
		return (NULL);
	res[i + 1] = 0;
	res = ft_fill(res, tab, i, n);
	return (res);
}
