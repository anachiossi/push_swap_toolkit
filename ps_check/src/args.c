#include "ps_check.h"

/* TODO: walk argv, fill args->push_swap on "--push-swap" (next argv), fill
 * args->ps_input on "--ps-input" (next argv). Return 1 if both were found,
 * 0 otherwise (missing/malformed args). */
int	parse_args(t_args *args, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	args->push_swap = NULL;
	args->ps_input = NULL;
	return (0);
}
