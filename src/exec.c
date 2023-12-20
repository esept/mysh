//
// Created by XuZY on 2023/11/25.
//

#include "../include/exec.h"
#include "../include/main.h"
#include "../include/prepro_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int exec_cmd(char * cmd[]){
	int status;
	pid_t pid;
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
//	getError("exec");
	return -1;
}

int command_cd(char *path){
	if (path == NULL){
		path = getenv("HOME");
	}
	if (chdir(path) != 0) getError("chdir");
	return 1;
}

void exec_pipe(char *cmd){
	int pipefd[2];
	pid_t pid;
	char *cmd_part;
	int fd_in = 0;

	while ((cmd_part = strtok_r(NULL, "|", &cmd))) {
		pipe(pipefd);
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) {
			dup2(fd_in, 0); // 复制 fd_in 到 stdin
			if (*cmd != '\0') {
				dup2(pipefd[1], 1); // 复制 stdout 到 pipefd[1]
			}
			close(pipefd[0]);

			char *args[SIZE];
			split_space(cmd_part, args); // 分割命令行参数
			if (execvp(args[0], args) == -1) {
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		} else {
			wait(NULL); // 等待子进程结束
			close(pipefd[1]);
			fd_in = pipefd[0]; // 将 pipefd[0] 保存为下一个命令的 stdin
		}
	}
}

