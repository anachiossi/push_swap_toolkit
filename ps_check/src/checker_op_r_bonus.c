/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_op_r_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 13:58:00 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/19 12:45:46 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

void	ckr_ra(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)b;
	r(a);
	c->ra++;
	c->total++;
}

void	ckr_rb(t_stack *a, t_stack *b, t_op_count *c)
{
	(void)a;
	r(b);
	c->rb++;
	c->total++;
}

void	ckr_rr(t_stack *a, t_stack *b, t_op_count *c)
{
	r(a);
	r(b);
	c->rr++;
	c->total++;
}
