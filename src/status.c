#include <stdio.h>
#include "status.h"

void set_last_terminated_process_status(int status, pid_t pid, char *cmd) {
	lastTerminatedProcess.status = status;
	lastTerminatedProcess.pid = pid;
	lastTerminatedProcess.cmd = cmd;
}

void display_last_process_status(terminatedProcess last) {
	pid_t pid = last.pid;
	char *cmd = last.cmd;
	int status = last.status;
	if (WIFEXITED(status)) {
        printf("`%d (%s)` terminé avec comme code de retour %d\n", pid, cmd, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("`%d (%s)` terminé anormalement (signal %d)\n", pid, cmd, WTERMSIG(status));
    }
}
