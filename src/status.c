/**
 * @file status.c
 * @brief Implementation of functions related to the last terminated process status.
 */

#include <stdio.h>
#include "status.h"
#include "error_handler.h"

/**
 * @brief Sets the status of the last terminated process.
 * 
 * This function sets the status, process ID, and command of the last terminated process.
 * 
 * @param status The status of the last terminated process.
 * @param pid The process ID of the last terminated process.
 * @param cmd The command of the last terminated process.
 */
void set_last_terminated_process_status(int status, pid_t pid, char *cmd) {
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
 */
void display_last_process_status(terminatedProcess last) {
	pid_t pid = last.pid;
	char *cmd = last.cmd;
	int status = last.status;
	if (WIFEXITED(status)) {
        printf(VERT("`%d (%s)` terminé avec comme code de retour %d\n"), pid, cmd, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf(ROUGE("`%d (%s)` terminé anormalement (signal %d)\n"), pid, cmd, WTERMSIG(status));
    }
}
