/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_rank.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 21:30:00 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/16 21:30:00 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

int	gen_rank(t_gen *g)
{
	g->rank = malloc(g->size * sizeof(int));
	if (!g->rank)
		return (0);
	if (g->dis_target < 0.5f)
		fill_ascending(g);
	else
		fill_descending(g);
	calc_disorder(g);
	if (g->dis_now < g->dis_target)
		make_chaos(g);
	else
		make_order(g);
	return (1);
}
