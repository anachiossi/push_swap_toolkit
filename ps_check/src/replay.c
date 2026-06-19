#include "ps_check.h"

/* TODO: implement the 9 physical moves against state->a/state->b (and their
 * _len counters), per the push_swap subject:
 *   sa  - swap the first 2 elements of a
 *   sb  - swap the first 2 elements of b
 *   ss  - sa then sb
 *   pa  - move top of b to top of a
 *   pb  - move top of a to top of b
 *   ra  - rotate a: top becomes bottom
 *   rb  - rotate b: top becomes bottom
 *   rra - reverse rotate a: bottom becomes top
 *   rrb - reverse rotate b: bottom becomes top
 * No-ops (e.g. sa on a size-1 stack) should be silently ignored, matching
 * push_swap's own behaviour. */
void	apply_move(t_state *state, const char *move)
{
	(void)state;
	(void)move;
}

/* TODO: if op is "rr", call apply_move(state, "ra") then apply_move(state,
 * "rb"); if op is "rrr", same with "rra"/"rrb"; otherwise apply_move(state,
 * op) directly. */
void	apply_op(t_state *state, const char *op)
{
	(void)state;
	(void)op;
}

/* TODO: return 0 if state->b_len != 0; otherwise return 1 iff state->a is
 * ascending from index 0 (top) to a_len - 1 (bottom). */
int	is_sorted(const t_state *state)
{
	(void)state;
	return (0);
}
