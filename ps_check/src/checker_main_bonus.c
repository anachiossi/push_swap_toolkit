/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_main_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 12:59:08 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/19 12:45:42 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

int		main(int argc, char **argv)
{
	t_stack		*a;
	t_stack		*b;
	t_ps_flags	f;
	int			i;

	if (argc < 2)
		return (0);
	a = new_stack();
	b = new_stack();
	i = assign_flags(&f, argv, argc);
	if (!a || !b || i < 0 || !input_to_stack(&argv[i], a))
		return (free_stack(a), free_stack(b), write(1, "Error\n", 6), 0);
	read_ops(a, b, f);
	print_result(a, b);
	return (free_stack(a), free_stack(b), 0);
}
