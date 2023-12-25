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
#include "../include/variable.h"

pid_t globalPID = 0;

void signalHandler(int sig){
	if (sig == SIGINT){
		// 检查当前运行进程
		if(globalPID != 0){//当前进程存在
			kill(globalPID,SIGINT);
		}else{
			printf("none\n");
			return;
		}
	}
}

int main() {
	char thiscmd[CMDLEN];
	char cwd[SIZE];

	init_variable();
//	init_env_variable();
	init_sharedMemeory();
	for (;;) {
//		signal(SIGINT,signalHandler);
		if (getcwd(cwd, sizeof(cwd)) == NULL) getError("getcwd");
		printf("%s -> ",cwd);
		if (fgets(thiscmd, CMDLEN - 1, stdin) == NULL) {
			perror("fgets: None input");
			continue;
		}
		size_t len = strlen(thiscmd);
		preprocess(thiscmd, len);
	}

}
