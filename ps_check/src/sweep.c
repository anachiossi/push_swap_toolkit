#include "ps_check.h"

/* Keep this grid in sync with ../config.py's SIZES/DISORDERS/ALGORITHMS,
 * which ps_graph reads -- a clean ps_check run should cover exactly what
 * gets graphed. Trim it down while developing; this full grid is large. */
static const int			g_sizes[] = {1, 5, 10, 20, 35, 50, 65, 80, 100, 150,
	200, 300, 500, 750, 1000};
static const double		g_disorders[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
	0.7, 0.8, 0.9, 1.0};
static const t_point		g_algo_template[] = {
	{0, 0.0, "adaptive", 0},
	{0, 0.0, "simple", 1},
	{0, 0.0, "medium", 2},
	{0, 0.0, "complex", 3},
};

/* TODO: build argv for ps_input (path, size, disorder, level, "0", "1"),
 * run_capture it, parse the last line into an int array. Build argv for
 * push_swap (path, "--<algo_name>", values...), run_capture it, split the
 * output into op tokens (one per non-empty line) and apply_op each onto a
 * fresh t_state seeded with the generated values in a, b empty. Return 0 if
 * is_sorted(&state), otherwise print what failed (point + final state) and
 * return non-zero. */
int	run_one_check(t_args *args, t_point point)
{
	(void)args;
	(void)point;
	return (0);
}

/* TODO: triple-nested loop over g_sizes / g_disorders / g_algo_template,
 * building a t_point for each combination and calling run_one_check. Print
 * progress as you go. Return the total number of failing points. */
int	run_sweep(t_args *args)
{
	(void)args;
	(void)g_sizes;
	(void)g_disorders;
	(void)g_algo_template;
	return (0);
}
