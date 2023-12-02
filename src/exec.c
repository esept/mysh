//
// Created by XuZY on 2023/11/25.
//

#include "../include/exec.h"
#include "../include/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Execute a command
 * @param cmd The command to be executed
 * @return The status of the command
 */
int exec_cmd(char * cmd[]){
	int status;
	pid_t pid, globalPID;
	pid = fork();
	if(pid == -1) getError("fork");
	else if(pid == 0){
		// child
		if (execvp(cmd[0],cmd) == ERR ) getError("exec");
	} else{
		globalPID = pid;
		waitpid(pid,&status,0);
		globalPID = 0;
		return WEXITSTATUS(status);
	}
	getError("exec");
}

/**
 * @brief Execute the cd command
 * @param path The path to change to
 * @return The status of the command
 */
int command_cd(char *path){
	if (path == NULL){
		path = getenv("HOME");
	}
	if (chdir(path) != 0) getError("chdir");
	return 1;
}

