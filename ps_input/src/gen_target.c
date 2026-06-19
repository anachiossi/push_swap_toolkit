/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_target.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 18:05:40 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/19 14:00:19 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

int	get_size(t_gen *g)
{
	char	*line;

	g->size = 0;
	while (g->size < 1 || g->size > 10000)
	{
		ft_printf("How many numbers? (1 - 10000)\n");
		line = get_next_line(0);
		if (!line)
			return (0);
		g->size = ft_atoi(line);
		free(line);
		if (g->size < 1 || g->size > 10000)
			ft_printf("Invalid size, enter a number between 1 and 10000\n");
	}
	return (1);
}

int	get_disorder(t_gen *g)
{
	char	*line;

	g->dis_target = -1;
	while (g->dis_target < 0.0f || g->dis_target > 1.0f)
	{
		ft_printf("How much disorder? (0.0 - 1.0)\n");
		line = get_next_line(0);
		if (!line)
			return (0);
		g->dis_target = (float)atof(line);
		free(line);
		if (g->dis_target < 0.0f || g->dis_target > 1.0f)
			ft_printf("Invalid, enter a number between 0.0 and 1.0\n");
	}
	return (1);
}

int	get_algorithm(t_gen *g)
{
	char	*line;

	g->level = -1;
	while (g->level == -1)
	{
		ft_printf("Which algorithm?\n(adaptive 0 | simple 1| medium 2| complex 3)\n");
		line = get_next_line(0);
		if (!line)
			return (0);
		if (!ft_strncmp(line, "0\n", 2))
			g->level = 0;
		else if (!ft_strncmp(line, "1\n", 2))
			g->level = 1;
		else if (!ft_strncmp(line, "2\n", 2))
			g->level = 2;
		else if (!ft_strncmp(line, "3\n", 2))
			g->level = 3;
		else
			ft_printf("Invalid: enter 0, 1, 2 or 3\n");
		free(line);
	}
	return (1);
}

int	get_bench(t_gen *g)
{
	char	*line;

	g->bench = -1;
	while (g->bench == -1)
	{
		ft_printf("Do you need benchmarks? (y / n)\n");
		line = get_next_line(0);
		if (!line)
			return (0);
		if (!ft_strncmp(line, "y\n", 2))
			g->bench = 1;
		else if (!ft_strncmp(line, "n\n", 2))
			g->bench = 0;
		else
			ft_printf("Invalid: enter y or n\n");
		free(line);
	}
	return (1);
}

int	get_rank_or_value(t_gen *g)
{
	char	*line;

	g->use_rank = -1;
	while (g->use_rank == -1)
	{
		ft_printf("Print rank or value?\n(r for rank (0 - n-1) | v for value (random int_min - int_max)\n");
		line = get_next_line(0);
		if (!line)
			return (0);
		if (!ft_strncmp(line, "r\n", 2))
			g->use_rank = 1;
		else if (!ft_strncmp(line, "v\n", 2))
			g->use_rank = 0;
		else
			ft_printf("Invalid: enter r or v\n");
		free(line);
	}
	return (1);
}
