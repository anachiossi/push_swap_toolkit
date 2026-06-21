/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 15:34:07 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:27 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps_check.h"

static void	init_ft_table(t_op *t)
{
	t[0].op_name = "sa\n";
	t[0].ft = ckr_sa;
	t[1].op_name = "sb\n";
	t[1].ft = ckr_sb;
	t[2].op_name = "ss\n";
	t[2].ft = ckr_ss;
	t[3].op_name = "pa\n";
	t[3].ft = ckr_pa;
	t[4].op_name = "pb\n";
	t[4].ft = ckr_pb;
	t[5].op_name = "ra\n";
	t[5].ft = ckr_ra;
	t[6].op_name = "rb\n";
	t[6].ft = ckr_rb;
	t[7].op_name = "rr\n";
	t[7].ft = ckr_rr;
	t[8].op_name = "rra\n";
	t[8].ft = ckr_rra;
	t[9].op_name = "rrb\n";
	t[9].ft = ckr_rrb;
	t[10].op_name = "rrr\n";
	t[10].ft = ckr_rrr;
	t[11].op_name = NULL;
	t[11].ft = NULL;
}

static int	run_op(char *op, t_stack *a, t_stack *b, t_op_count *c)
{
	static t_op	ops[12];
	static int	ready;
	int			i;

	if (!ready)
	{
		init_ft_table(ops);
		ready = 1;
	}
	i = 0;
	while (ops[i].op_name != NULL)
	{
		if (!ft_strncmp(op, ops[i].op_name, ft_strlen(ops[i].op_name)))
			return (ops[i].ft(a, b, c), 1);
		i++;
	}
	return (0);
}

void	read_ops(t_stack *a, t_stack *b, t_ps_flags f)
{
	static t_op_count	c;
	float				d;
	char				*op;

	d = disorder(a);
	op = get_next_line(0);
	while (op)
	{
		if (!run_op(op, a, b, &c))
			return (free(op), ft_putstr_fd("Error\n", 2));
		free(op);
		op = get_next_line(0);
	}
	print_check_bench(d, f, &c);
}
