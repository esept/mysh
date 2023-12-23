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

pid_t globalPID = 0;
pid_t bg_processes[MAX_BG_PROCESSES];
int last_bg_process_index = -1;
terminatedProcess lastTerminatedProcess = {0, 0, NULL};

void killBackgroundProcesses() {
    for (int i = 0; i <= last_bg_process_index; i++) {
        kill(bg_processes[i], SIGKILL);
    }
}

// for each new process: bg_processes[bg_process_count++] = getpid();
/**
 * @brief Kill all background processes before quit
 * @param sig signal number
 * @return void
*/
void signalHandler(int sig) {
    char c;
    signal(sig, SIG_IGN);
    printf(" Do you really want to quit? [y/n] ");
    c = tolower(getchar());

	// fflush(stdin); // Have to press enter 2 times (instead of one) when not 'y'

    if (c != 'y') {
        signal(SIGINT, signalHandler);
		return;
    } 
	killBackgroundProcesses();
    exit(EXIT_SUCCESS);
}

int main() {
	signal(SIGINT,signalHandler);
	char thiscmd[CMDLEN];
	char cwd[SIZE];

	for (;;) {
		if (getcwd(cwd, sizeof(cwd)) == NULL) getError("getcwd");
		printf("%s ~> ",cwd);
		if (fgets(thiscmd, CMDLEN - 1, stdin) == NULL) {
			perror("fgets: None input");
			continue;
		}
		size_t len = strlen(thiscmd);
		preprocess(thiscmd, len);
		// split(thiscmd,len);
	}

	return 0;
}
