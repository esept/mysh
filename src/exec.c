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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

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

int cmd_pipe(char *argv[], int argc) {
	int num_pipes = 0;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "|") == 0) {
			num_pipes++;
		}
	}

	int pipefds[2 * num_pipes];

	// 创建管道
	for (int i = 0; i < num_pipes; i++) {
		if (pipe(pipefds + i * 2) < 0) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	int j = 0;
	int start = 0;
	pid_t pid;

	while (start < argc) {
		// 寻找下一个管道标记或命令末尾
		while (j < argc && strcmp(argv[j], "|") != 0) {
			j++;
		}

		argv[j] = NULL; // 将管道标记替换为NULL，以便execvp调用

		pid = fork();
		if (pid == 0) {
			// 子进程
			// 如果不是第一个命令
			if (start != 0) {
				dup2(pipefds[(start / 2 - 1) * 2], STDIN_FILENO);
			}
			// 如果不是最后一个命令
			if (j < argc - 1) {
				dup2(pipefds[(start / 2) * 2 + 1], STDOUT_FILENO);
			}

			for (int i = 0; i < 2 * num_pipes; i++) {
				close(pipefds[i]);
			}

			execvp(argv[start], &argv[start]);
			perror("execvp");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		start = j + 1;
		j++;
	}

	// 父进程关闭所有管道
	for (int i = 0; i < 2 * num_pipes; i++) {
		close(pipefds[i]);
	}

	// 等待所有子进程
	for (int i = 0; i <= num_pipes; i++) {
		wait(NULL);
	}

	return 1;
} // without builtin function

int cmd_pipe2(char *argv[], int argc) {
	int num_pipes = 0;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "|") == 0) {
			num_pipes++;
		}
	}

	int pipefds[2 * num_pipes];

	// 创建管道
	for (int i = 0; i < num_pipes; i++) {
		if (pipe(pipefds + i * 2) < 0) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	int j = 0;
	int start = 0;
	pid_t pid;

	while (start < argc) {
		// 寻找下一个管道标记或命令末尾
		while (j < argc && strcmp(argv[j], "|") != 0) {
			j++;
		}

		argv[j] = NULL; // 将管道标记替换为NULL

		// 检查是否为内置命令
		if (is_builtin_command(argv[start])) {
			// 执行内置命令，如需要，应处理管道
			printf("yes\n\n");
			execute_builtin_command(&argv[start], pipefds, start, num_pipes);
		} else {
			// 执行外部命令
			pid = fork();
			if (pid == 0) {
				// 子进程
				// 如果不是第一个命令
				if (start != 0) {
					dup2(pipefds[(start / 2 - 1) * 2], STDIN_FILENO);
				}
				// 如果不是最后一个命令
				if (j < argc - 1) {
					dup2(pipefds[(start / 2) * 2 + 1], STDOUT_FILENO);
				}

				for (int i = 0; i < 2 * num_pipes; i++) {
					close(pipefds[i]);
				}

				execvp(argv[start], &argv[start]);
				perror("execvp");
				exit(EXIT_FAILURE);
			} else if (pid < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
		}

		start = j + 1;
		j++;
	}

	// 父进程关闭所有管道
	for (int i = 0; i < 2 * num_pipes; i++) {
		close(pipefds[i]);
	}

	// 等待所有子进程
	for (int i = 0; i <= num_pipes; i++) {
		wait(NULL);
	}

	return 1;
}

int cmd_redi(char *argv[], int argc) {
	int save_stdout = dup(STDOUT_FILENO);
	int save_stderr = dup(STDERR_FILENO);
	int save_stdin = dup(STDIN_FILENO);
	int fd;
	char *new_argv[argc]; // 创建新的参数数组
	int new_argc = 0;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], ">") == 0) {
			fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			i++; // 跳过文件名
		} else if (strcmp(argv[i], ">>") == 0) {
			fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			i++;
		} else if (strcmp(argv[i], "2>") == 0) {
			fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDERR_FILENO);
			close(fd);
			i++;
		} else if (strcmp(argv[i], "2>>") == 0) {
			fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDERR_FILENO);
			close(fd);
			i++;
		} else if (strcmp(argv[i], "<") == 0){
			fd = open(argv[i + 1],O_RDONLY);
			dup2(fd,STDIN_FILENO);
			close(fd);
			i++;
		} else if (strcmp(argv[i], ">>&") == 0 || strcmp(argv[i], ">&") == 0) {
			fd = open(argv[i + 1], (strcmp(argv[i], ">>&") == 0) ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC), 0644);
			if (fd == -1) {
				perror("open");
				return -1;
			}
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
			argv[i] = NULL; // 从此处开始移除重定向符号及其后的文件名
			argv[i + 1] = NULL;
			break; // 假设处理了第一个重定向符号
		}else {
			new_argv[new_argc++] = argv[i]; // 保留非重定向参数
		}
	}
	new_argv[new_argc] = NULL; // 结束新的参数数组

	pid_t pid = fork();
	if (pid == 0) {
		// 子进程
		execvp(new_argv[0], new_argv);
		perror("execvp");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		// 父进程
		int status;
		waitpid(pid, &status, 0);
	} else {
		perror("fork");
		return -1;
	}

	// 恢复原始的标准输出和标准错误
	dup2(save_stdout, STDOUT_FILENO);
	dup2(save_stderr, STDERR_FILENO);
	dup2(save_stdin, STDIN_FILENO); // 恢复标准输入
	close(save_stdout);
	close(save_stderr);
	close(save_stdin);
	return 0;
} // redirect

