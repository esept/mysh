#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
    int status;
	pid_t pid;
 	char *cmd;
} terminatedProcess;

extern terminatedProcess lastTerminatedProcess;

// To keep info about the last terminated process updated:
// waitpid(pid,&status,0);
// set_last_terminated_process_status(status, pid, cmd[0]);

void set_last_terminated_process_status(int status, pid_t pid, char *cmd);
void display_last_process_status(terminatedProcess last);

#endif
