/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsantoni <gsantoni@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:29:22 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/14 11:22:44 by gsantoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>

int					ft_printf(const char *format, ...);

typedef struct s_node
{
	int				value;
	int				rank;
	struct s_node	*next;
}					t_node;

typedef struct s_stack
{
	t_node			*top;
	t_node			*bottom;
	int				size;
}					t_stack;

// level if 0 == adaptive, 1 == simple, 2 == medium, 3 == complex
// bench is boolean 1 if --bench is set, 0 otherwise
typedef struct s_ps_flags
{
	int				level;
	int				bench;
}					t_ps_flags;

// parsing — parse.c (ft_atoi from libft)
int					is_valid(char *s);
int					input_to_stack(char **arg, t_stack *a);
int					assign_flags(t_ps_flags *f, char **argv, int argc);
void				assign_ranks(t_stack *a);
void				benchmode(float d, int *n, int level);

// stack creation — stack.c
t_node				*new_node(int value);
t_stack				*new_stack(void);
void				free_stack(t_stack *stack);
void				print_stack(t_stack *stack);

// stack manipulation — stack_utils.c
void				push(t_stack *stack, t_node *new);
t_node				*pop(t_stack *stack);
void				push_bottom(t_stack *stack, t_node *new);
t_node				*pop_bottom(t_stack *stack);

// base operations — operations_base.c
int					*op_count(int k);
void				p(t_stack *x, t_stack *y);
void				r(t_stack *x);
void				r_r(t_stack *x);
void				s(t_stack *x);

// push swap operations — operations_ps.c
void				sa(int bench, t_stack *a);
void				sb(int bench, t_stack *b);
void				ss(int bench, t_stack *a, t_stack *b);
void				pa(int bench, t_stack *a, t_stack *b);
void				pb(int bench, t_stack *a, t_stack *b);

// rotate operations — operations_r.c
void				ra(int bench, t_stack *a);
void				rb(int bench, t_stack *b);
void				rr(int bench, t_stack *a, t_stack *b);

// reverse rotate operations — operations_rr.c
void				rra(int bench, t_stack *a);
void				rrb(int bench, t_stack *b);
void				rrr(int bench, t_stack *a, t_stack *b);

// adaptive algorithm & flag — dispatcher.c
void				dispatcher(t_stack *a, t_stack *b, t_ps_flags f);
float				disorder(t_stack *h);
int					is_sorted(t_stack *a);
void				adaptive_sort(t_stack *a, t_stack *b, t_ps_flags f);

// sorting — sort.c
void				selection_sort(int bench, t_stack *a, t_stack *b);
void				chunk_sort(int bench, t_stack *a, t_stack *b); // to do
void				radix_sort(int bench, t_stack *a, t_stack *b);

// sorting simple functions - sort_utils.c
void				sort_inverted(int bench, t_stack *a, t_stack *b);
void				min_to_top(int bench, t_stack *a);
int					find_min(t_stack *x);
void				sort_2(int bench, t_stack *a);
void				sort_3(int bench, t_stack *a);

#endif
