/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 12:59:11 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:32 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

static void	bench_s(t_op_count *c)
{
	ft_putstr_fd("[bench] sa: ", 2);
	ft_putnbr_fd(c->sa, 2);
	ft_putstr_fd("\tsb: ", 2);
	ft_putnbr_fd(c->sb, 2);
	ft_putstr_fd("\tss: ", 2);
	ft_putnbr_fd(c->ss, 2);
	ft_putstr_fd("\tpa: ", 2);
	ft_putnbr_fd(c->pa, 2);
	ft_putstr_fd("\tpb: ", 2);
	ft_putnbr_fd(c->pb, 2);
	ft_putchar_fd('\n', 2);
}

static void	bench_r(t_op_count *c)
{
	ft_putstr_fd("[bench] ra: ", 2);
	ft_putnbr_fd(c->ra, 2);
	ft_putstr_fd("\trb: ", 2);
	ft_putnbr_fd(c->rb, 2);
	ft_putstr_fd("\trr: ", 2);
	ft_putnbr_fd(c->rr, 2);
	ft_putstr_fd("\trra: ", 2);
	ft_putnbr_fd(c->rra, 2);
	ft_putstr_fd("\trrb: ", 2);
	ft_putnbr_fd(c->rrb, 2);
	ft_putstr_fd("\trrr: ", 2);
	ft_putnbr_fd(c->rrr, 2);
	ft_putchar_fd('\n', 2);
}

void	print_check_bench(float d, t_ps_flags f, t_op_count *c)
{
	if (!f.bench)
		return ;
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putfloat_fd(d * 100, 2, 2);
	ft_putstr_fd("%\n[bench] total ops: ", 2);
	ft_putnbr_fd(c->total, 2);
	ft_putchar_fd('\n', 2);
	bench_s(c);
	bench_r(c);
}

void	print_result(t_stack *a, t_stack *b)
{
	if (is_sorted(a) && b->size == 0)
		ft_printf("OK\n");
	else
		ft_printf("KO\n");
}
