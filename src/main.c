//
// Created by XuZY on 2023/11/25.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


#include "../include/prepro_input.h"
#include "../include/main.h"
#include "status.h"
#include "../include/variable.h"

pid_t globalPID = 0;
terminatedProcess lastTerminatedProcess = {0, 0, NULL};

int last_job_index = -1;
job bg_jobs[MAX_BG_PROCESSES];

void killBackgroundProcesses() {
    for (int i = 0; i <= last_job_index; i++) {
        kill(bg_jobs[i].pid, SIGKILL);
    }
}

// To track all processes, each new process --> add_job
/**
 * @brief Kill all background processes before quit
 * @param sig signal number
 * @return void
*/
void signalHandler(int sig) {
    char c;
    signal(sig, SIG_IGN);
    printf(" Do you really want to quit? [y/n] ");
    scanf(" %c", &c);
    c = tolower(c);

    if (c != 'y') {
        signal(SIGINT, signalHandler);
        return;
    } 
    killBackgroundProcesses();
    clean_env_variable();
    clean_local_variable();
    exit(EXIT_SUCCESS);
}

void add_job(pid_t pid, char* command) {
    last_job_index++;
    bg_jobs[last_job_index].job_number = last_job_index;
    bg_jobs[last_job_index].pid = pid;
    bg_jobs[last_job_index].status = "En cours";
    bg_jobs[last_job_index].command = command;
}

void update_jobs() {
    for (int i = 0; i < last_job_index; i++) {
        int status;
        pid_t result = waitpid(bg_jobs[i].pid, &status, WNOHANG);
        if (result == 0) {
            bg_jobs[i].status = "En cours";
        } else if (result == bg_jobs[i].pid) {
            bg_jobs[i].status = "Stoppé";
        }
    }
}

int print_jobs() {
    update_jobs();
    for (int i = 0; i < last_job_index; i++) {
        printf("[%d] %d %s %s\n", bg_jobs[i].job_number, bg_jobs[i].pid, bg_jobs[i].status, bg_jobs[i].command);
    }

    return 0;
}

int main() {
	signal(SIGINT,signalHandler);
	char thiscmd[CMDLEN];
	char cwd[SIZE];

	init_variable();
	init_sharedMemeory();
	for (;;) {
		if (getcwd(cwd, sizeof(cwd)) == NULL) getError("getcwd");
		printf("%s ~> ",cwd);
		if (fgets(thiscmd, CMDLEN - 1, stdin) == NULL) {
			perror("fgets: None input");
			continue;
		}
		size_t len = strlen(thiscmd);
		preprocess(thiscmd, len);
	}

	return 0;
}
