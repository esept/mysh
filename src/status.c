/**
 * @file status.c
 * @brief Implementation of functions related to the last terminated process status.
 */

#include <stdio.h>
#include "status.h"
#include "error_handler.h"

terminatedProcess lastTerminatedProcess = {0, 0, NULL};

// in the parent of each process, we call (depending on the case):
// waitpid(pid, &status, 0);
// OR
// pid = waitpid(-1, &status, 0);
// set_last_terminated_process_status(status, pid, cmd[0]);
// OR
// pid = waitpid(-1, &status, 0);
// set_last_terminated_process_status(status, pid, "my cmd name");
/**
 * @brief Sets the status of the last terminated process.
 *
 * This function sets the status, process ID, and command of the last terminated process.
 *
 * @param status The status of the last terminated process.
 * @param pid The process ID of the last terminated process.
 * @param cmd The command of the last terminated process.
 */
void set_last_terminated_process_status(int status, pid_t pid, char *cmd)
{
	// printf("(set_last_terminated_process_status) cmd: %s\n", cmd);

	lastTerminatedProcess.status = status;
	lastTerminatedProcess.pid = pid;
	lastTerminatedProcess.cmd = cmd;
}

/**
 * @brief Displays the status of the last terminated process.
 *
 * This function displays the process ID, command, and termination status of the last terminated process.
 * If the process exited normally, it prints the exit status.
 * If the process was terminated by a signal, it prints the signal number.
 *
 * @param last The last terminated process.
 * @return 0 if the process exited normally, -1 if it was terminated anormally.
 */
int display_last_process_status()
{
	pid_t pid = lastTerminatedProcess.pid;
	char *cmd = lastTerminatedProcess.cmd;
	int status = lastTerminatedProcess.status;
	if (cmd == NULL)
	{
		printf(VERT("No terminated process.\n"));
		return 0;
	}


	if (WIFEXITED(status))
	{
		printf(VERT("`%d` terminé avec comme code de retour %d\n"), pid, WEXITSTATUS(status));
		return 0;
	}
	else if (WIFSIGNALED(status))
	{
		printf(ROUGE("`%d` terminé anormalement (signal %d)\n"), pid, WTERMSIG(status));
	}
	
	return -1;
}
