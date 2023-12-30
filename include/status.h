#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// To keep info about the last terminated process updated:
typedef struct {
    int status;
	pid_t pid;
 	char *cmd;
} terminatedProcess;

extern terminatedProcess lastTerminatedProcess;

void set_last_terminated_process_status(int status, pid_t pid, char *cmd);
int display_last_process_status();

#endif
