//
// Created by XuZY on 2023/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>
#include <regex.h>
#include <sys/stat.h>
#include "main.h"

#define SUPPORTED_WILDCARDS ".*?[]{}^~"
#define MAX_CMD_LENGTH 512

int containsSupportedWildcard(const char *str) {
    const char *supportedWildcards = SUPPORTED_WILDCARDS;

    while (*supportedWildcards) {
        if (strchr(str, *supportedWildcards) != NULL) {
            return 1; // At least one supported wildcard found
        }
        supportedWildcards++;
    }

    return 0; // No supported wildcard found
}

int getRegexIndex(char *cmd[]) {
	int i = 1;
	while (cmd[i] != NULL) {
		if (containsSupportedWildcard(cmd[i])) {
			return i;
		}
		i++;
	}
	return -1;
}

int exec_cmd(char * cmd[]){
	int regexIndex = -1;
	int status;
	pid_t pid;
	pid = fork();
	if(pid == -1) 
		getError("fork");
	else if(pid == 0){
		// child
		bg_processes[bg_process_count++] = getpid();
		regexIndex = getRegexIndex(cmd);
		if (regexIndex != -1) {
            glob_t glob_result;
            if (glob(cmd[regexIndex], GLOB_NOCHECK, NULL, &glob_result) != 0) {
                perror("Erreur lors de l'évaluation des wildcards");
                exit(EXIT_FAILURE);
            }

            // check if at least one file matches the wildcard
            if (glob_result.gl_pathc > 0) {
				// Calculate the new length of the command
                size_t newCmdLength = regexIndex + glob_result.gl_pathc + 1;

                // Allocate memory for the new command
                char **newCmd = malloc(newCmdLength * sizeof(char *));
                if (newCmd == NULL) {
                    perror("Erreur d'allocation mémoire");
                    exit(EXIT_FAILURE);
                }

                // Copy the first elements of the command
                for (size_t i = 0; i < regexIndex; i++) {
                    newCmd[i] = cmd[i];
                }

                // Copy the files matching the wildcard
                for (size_t j = 0; j < glob_result.gl_pathc; j++) {
                    newCmd[regexIndex + j] = strdup(glob_result.gl_pathv[j]);
                }

                // Add NULL at the end of the new command
                newCmd[newCmdLength - 1] = NULL;

                // Replace the old command with the new one
                cmd = newCmd;

                if (execvp(cmd[0], cmd) == ERR) {
                    perror("Erreur lors de l'exécution de la commande");
                    exit(EXIT_FAILURE);
                }
            } else {
				for (size_t j = 0; j < glob_result.gl_pathc; j++) {
                    fprintf(stderr, "0 %s\n", glob_result.gl_pathv[j]);
                }
                exit(EXIT_SUCCESS);
            }

            // cleanup glob_t
            globfree(&glob_result);
        } else {
            if (execvp(cmd[0], cmd) == ERR) getError("exec");
        }
	} else{
		bg_processes[bg_process_count++] = getpid();
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
			bg_processes[bg_process_count++] = getpid();
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
			bg_processes[bg_process_count++] = getpid();
			wait(NULL); // 等待子进程结束
			close(pipefd[1]);
			fd_in = pipefd[0]; // 将 pipefd[0] 保存为下一个命令的 stdin
		}
	}
}

