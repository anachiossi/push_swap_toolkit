/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_number.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 18:04:22 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/14 22:59:03 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_input.h"

float	calc_disorder(t_gen *g)
{
	int	i;
	int	j;
	int	wrong;
	int	pairs;

	if (!g || g->size < 2)
		return (0.0f);
	wrong = 0;
	pairs = g->size * (g->size - 1) / 2;
	i = 0;
	while (i < g->size - 1)
	{
		j = i + 1;
		while (j < g->size)
		{
			wrong += (g->rank[i] > g->rank[j]);
			j++;
		}
		i++;
	}
	g->dis_now = (float)wrong / (float)pairs;
	return (g->dis_now);
}

static void	sort(int *arr, int size)
{
	int	i;
	int	j;
	int	tmp;

	i = 1;
	while (i < size)
	{
		tmp = arr[i];
		j = i - 1;
		while (j >= 0 && arr[j] > tmp)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tmp;
		i++;
	}
}

static void	fill_random(int *arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		arr[i] = rand() - RAND_MAX / 2;
		i++;
	}
}

static void	put_value(t_gen *g, int *temp)
{
	int	i;

	i = 0;
	while (i < g->size)
	{
		g->value[i] = temp[g->rank[i]];
		i++;
	}
}

int	gen_value(t_gen *g)
{
	int	*temp;

	g->value = malloc(g->size * sizeof(int));
	if (!g->value)
		return (0);
	temp = malloc(g->size * sizeof(int));
	if (!temp)
		return (free(g->value), g->value = NULL, 0);
	fill_random(temp, g->size);
	sort(temp, g->size);
	put_value(g, temp);
	free(temp);
	return (1);
}
