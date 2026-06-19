#ifndef PS_CHECK_H
# define PS_CHECK_H

# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAX_STACK 10000

typedef struct s_args
{
	char	*push_swap;	/* path to the binary under test */
	char	*ps_input;	/* path to ps_input (formerly ps_gen) */
}			t_args;

/* One push_swap stack state. index 0 = top of each stack, mirrors the
 * convention used by push_swap itself and by apps/shared/stack_engine.js. */
typedef struct s_state
{
	int		a[MAX_STACK];
	int		a_len;
	int		b[MAX_STACK];
	int		b_len;
}			t_state;

/* One point in the sweep grid: a size/disorder/algorithm combination to
 * generate, run, and verify. */
typedef struct s_point
{
	int			size;
	double		disorder;
	const char	*algo_name;	/* e.g. "simple", passed as --simple */
	int			algo_level;	/* level argument forwarded to ps_input */
}				t_point;

/* args.c -- CLI parsing: --push-swap PATH --ps-input PATH */
int		parse_args(t_args *args, int argc, char **argv);

/* proc.c -- run a child process and capture its stdout.
 * TODO: fork, dup2 a pipe's write end onto STDOUT_FILENO in the child,
 * execvp(argv[0], argv), read the pipe in the parent until EOF, waitpid,
 * return the captured text as a malloc'd string (caller frees). */
char	*run_capture(char *const argv[]);

/* replay.c -- independent reimplementation of the 9 push_swap operations,
 * built from the subject's definitions (sa/sb/ss/pa/pb/ra/rb/rra/rrb), not
 * from push_swap's own operations_*.c. Keep it that way: if this ever calls
 * into push_swap's own object files, the checker stops being independent.
 *
 * apply_move applies exactly one physical move to state.
 * apply_op applies one printed token, expanding combined tokens (rr -> ra,
 * rb ; rrr -> rra, rrb) into their physical moves via apply_move.
 * is_sorted returns 1 iff b is empty and a is ascending from top to bottom.
 */
void	apply_move(t_state *state, const char *move);
void	apply_op(t_state *state, const char *op);
int		is_sorted(const t_state *state);

/* sweep.c -- the correctness sweep itself.
 * run_one_check: generate a stack via ps_input for `point`, run push_swap
 * on it, replay the printed op list with apply_op, and report whether the
 * result is correctly sorted. Return 0 on pass, non-zero on failure (and
 * print/record what failed -- caller decides the report format).
 * run_sweep: iterate the grid (see SIZES/DISORDERS/ALGORITHMS in sweep.c,
 * keep them in sync with ../config.py's grid used by ps_graph) and call
 * run_one_check for every point. Return the number of failures found. */
int		run_one_check(t_args *args, t_point point);
int		run_sweep(t_args *args);

#endif
