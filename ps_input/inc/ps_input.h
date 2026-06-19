/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_input.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 16:40:40 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/13 20:40:45 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS_INPUT_H
# define PS_INPUT_H

# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <time.h>

typedef struct s_gen
{
	int		size;
	float	dis_target;
	float	dis_now;
	int		level;
	int		bench;
	int		use_rank;
	int		*rank;
	int		*value;
}			t_gen;

/* gen_target.c */
int			get_size(t_gen *g);
int			get_disorder(t_gen *g);
int			get_algorithm(t_gen *g);
int			get_bench(t_gen *g);

/* gen_rank.c */
int			gen_rank(t_gen *g);

/* gen_agents.c */
void		fill_ascending(t_gen *g);
void		fill_descending(t_gen *g);
void		make_chaos(t_gen *g);
void		make_order(t_gen *g);

/* gen_number.c */
float		calc_disorder(t_gen *g);
int			gen_value(t_gen *g);

/* gen_text.c */
void		print_benchmark(t_gen *g);
void		print_input(t_gen *g);

/*main.c*/
int			get_rank_or_value(t_gen *g);

#endif
