/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 16:23:00 by jtaravel          #+#    #+#             */
/*   Updated: 2023/04/10 17:03:34 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

#define STRUCT_H

typedef struct s_syntax
{
	int			sq_opened;
	int			dq_opened;
	int			braces;
}	t_s;

#endif
