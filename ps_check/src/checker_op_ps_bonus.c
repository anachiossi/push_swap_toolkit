/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_op_ps_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:29:16 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:29 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

void	ckr_sa(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)b;
	s(a);
	c->sa++;
	c->total++;
}

void	ckr_sb(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)a;
	s(b);
	c->sb++;
	c->total++;
}

void	ckr_ss(t_stack *a, t_stack *b, t_op_count *c)
{
	s(a);
	s(b);
	c->ss++;
	c->total++;
}

void	ckr_pa(t_stack *a, t_stack *b, t_op_count *c)
{
	p(b, a);
	c->pa++;
	c->total++;
}

void	ckr_pb(t_stack *a, t_stack *b, t_op_count *c)
{
	p(a, b);
	c->pb++;
	c->total++;
}
