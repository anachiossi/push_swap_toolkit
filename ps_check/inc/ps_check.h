/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_checker.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 17:26:59 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/19 12:45:21 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS_CHECK_H
# define PS_CHECK_H

# include "push_swap.h"

//counter of each operation and total 
typedef struct s_op_count
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
	int	total;
}	t_op_count;

//i will make an array of this structs so that i can call each op function
typedef struct s_op
{
	char	*op_name;
	void	(*ft)(t_stack *, t_stack *, t_op_count *);
}	t_op;

// operations functions that do not print but execute and count
void	ckr_sa(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_sb(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_ss(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_pa(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_pb(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_ra(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_rb(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_rr(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_rra(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_rrb(t_stack *a, t_stack *b, t_op_count *c);
void	ckr_rrr(t_stack *a, t_stack *b, t_op_count *c);

// checker logic — checker_bonus.c
void	read_ops(t_stack *a, t_stack *b, t_ps_flags f);

// bench output — checker_utils_bonus.c
void	print_result(t_stack *a, t_stack *b);
void	print_check_bench(float d, t_ps_flags f, t_op_count *c);

#endif
