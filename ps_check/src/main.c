#include "ps_check.h"

int	main(int argc, char **argv)
{
	t_args	args;
	int		failures;

	if (!parse_args(&args, argc, argv))
	{
		ft_printf("Usage: ps_check --push-swap PATH --ps-input PATH\n");
		return (1);
	}
	failures = run_sweep(&args);
	if (failures > 0)
		return (1);
	return (0);
}
