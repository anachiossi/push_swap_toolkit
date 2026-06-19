/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 18:58:42 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/19 14:10:02 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

static int	init_targets(t_gen *g)
{
	if (!get_size(g))
		return (0);
	if (!get_disorder(g))
		return (0);
	if (!get_algorithm(g))
		return (0);
	if (!get_bench(g))
		return (0);
	if (!get_rank_or_value(g))
		return (0);
	return (1);
}

int	init_targets_auto(t_gen *g, char **argv)
{
	g->size = ft_atoi(argv[1]);
	g->dis_target = (float)atof(argv[2]);
	g->level = ft_atoi(argv[3]);
	if (argv[4][0] == 'y')
		g->bench = 1;
	else if (argv[4][0] == 'n')
		g->bench = 0;
	else
		return (0);
	if (argv[5][0] == 'r')
		g->use_rank = 1;
	else if (argv[5][0] == 'v')
		g->use_rank = 0;
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_gen	g;

	if (argc != 1 && argc != 6)
	{
		ft_printf("Usage: ps_gen [size(0-10000) | disorder(0.0-1.0) "
			"| level(0-3) | bench(y/n) | use_rank(r/v)]\n");
		return (1);
	}
	srand(time(NULL) ^ (unsigned int)getpid());
	if (argc == 1)
	{
		ft_printf("Push Swap input generator\n");
		if (!init_targets(&g))
			return (1);
	}
	else if (!init_targets_auto(&g, argv))
	{
		ft_printf("Usage: ps_gen [size(0-10000) | disorder(0.0-1.0) "
			"| level(0-3) | bench(y/n) | use_rank(r/v)]\n");
		return (1);
	}
	if (!gen_rank(&g))
		return (1);
	if (!gen_value(&g))
		return (free(g.rank), 1);
	calc_disorder(&g);
	print_input(&g);
	return (free(g.rank), free(g.value), 0);
}
