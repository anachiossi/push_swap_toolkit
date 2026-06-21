/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_op_rr_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 18:13:05 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:31 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

void	ckr_rra(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)b;
	r_r(a);
	c->rra++;
	c->total++;
}

void	ckr_rrb(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)a;
	r_r(b);
	c->rrb++;
	c->total++;
}

void	ckr_rrr(t_stack *a, t_stack *b, t_op_count *c)
{
	r_r(a);
	r_r(b);
	c->rrr++;
	c->total++;
}
