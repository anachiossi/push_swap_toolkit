#include "ps_check.h"

/* TODO: pipe(), fork(); in the child dup2(pipe_write, STDOUT_FILENO), close
 * both pipe ends, execvp(argv[0], argv) and _exit(127) on failure; in the
 * parent close the write end, read() the read end in a loop into a growing
 * malloc'd buffer until EOF, waitpid() the child, close the read end, and
 * return the buffer (NUL-terminated, caller frees). Return NULL on any
 * fork/pipe/exec failure. */
char	*run_capture(char *const argv[])
{
	(void)argv;
	return (NULL);
}
