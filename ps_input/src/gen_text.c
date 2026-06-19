/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_text.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 21:18:17 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/17 16:07:28 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

static void	print_flags(t_gen *g)
{
	if (g->level == 0)
		ft_printf("--adaptive ");
	if (g->level == 1)
		ft_printf("--simple ");
	if (g->level == 2)
		ft_printf("--medium ");
	if (g->level == 3)
		ft_printf("--complex ");
	if (g->bench)
		ft_printf("--bench ");
}

static void	print_array(int *arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		ft_printf("%d", arr[i]);
		if (i < size - 1)
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

void	print_benchmark(t_gen *g)
{
	ft_printf("size               : %d\n", g->size);
	ft_printf("disorder target    : %.4f\n", g->dis_target);
	ft_printf("disorder generated : %.4f\n", g->dis_now);
}

void	print_input(t_gen *g)
{
	print_flags(g);
	if (g->use_rank)
		print_array(g->rank, g->size);
	else
		print_array(g->value, g->size);
}
