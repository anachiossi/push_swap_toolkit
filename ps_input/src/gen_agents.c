/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_agents.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 18:05:06 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/14 22:58:53 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

static void	swap(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	fill_ascending(t_gen *g)
{
	int	i;

	i = 0;
	while (i < g->size)
	{
		g->rank[i] = i;
		i++;
	}
}

void	fill_descending(t_gen *g)
{
	int	i;

	i = 0;
	while (i < g->size)
	{
		g->rank[i] = g->size - 1 - i;
		i++;
	}
}

void	make_chaos(t_gen *g)
{
	int	pairs;
	int	to_chaos;
	int	i;

	pairs = g->size * (g->size - 1) / 2;
	to_chaos = (int)round((g->dis_target - g->dis_now) * pairs);
	while (to_chaos > 0)
	{
		i = rand() % (g->size - 1);
		if (g->rank[i] < g->rank[i + 1])
		{
			swap(&g->rank[i], &g->rank[i + 1]);
			to_chaos--;
		}
	}
}

void	make_order(t_gen *g)
{
	int	pairs;
	int	to_order;
	int	i;

	pairs = g->size * (g->size - 1) / 2;
	to_order = (int)round((g->dis_now - g->dis_target) * pairs);
	while (to_order > 0)
	{
		i = rand() % (g->size - 1);
		if (g->rank[i] > g->rank[i + 1])
		{
			swap(&g->rank[i], &g->rank[i + 1]);
			to_order--;
		}
	}
}
