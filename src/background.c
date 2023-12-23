////
//// Created by XuZY on 2023/12/23.
////
//
//#include "background.h"
//#include "main.h"
//
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <signal.h>
//
//// 全局变量用于存储后台进程信息
//typedef struct {
//	pid_t pid;       // 进程ID
//	int job_number;  // 作业号
//	char cmd[CMDLEN];// 命令文本
//} background_process;
//
//background_process bg_processes[SIZE]; // 假设SIZE足够大
//int num_bg_processes = 0;               // 当前后台进程数量
//
//// 向bg_processes数组中添加新的后台进程信息
//void add_bg_process(pid_t pid, char *cmd) {
//	if (num_bg_processes < SIZE) {
//		bg_processes[num_bg_processes].pid = pid;
//		bg_processes[num_bg_processes].job_number = num_bg_processes + 1;
//		strncpy(bg_processes[num_bg_processes].cmd, cmd, CMDLEN);
//		num_bg_processes++;
//	}
//	// 你可能还想添加一些错误处理，以防超出数组大小
//}
//
//// 从bg_processes数组中移除已经完成的后台进程
//void remove_bg_process(pid_t pid) {
//	for (int i = 0; i < num_bg_processes; i++) {
//		if (bg_processes[i].pid == pid) {
//			// 移动数组中的元素来覆盖已完成的进程信息
//			for (int j = i; j < num_bg_processes - 1; j++) {
//				bg_processes[j] = bg_processes[j + 1];
//			}
//			num_bg_processes--;
//			break;
//		}
//	}
//}
//
//// 实现 cmd_back 函数
//int cmd_back(char *argv[], int argc) {
//	pid_t pid = fork();
//	if (pid == -1) {
//		perror("fork");
//		return -1;
//	} else if (pid == 0) {
//		// 子进程
//		execvp(argv[0], argv);
//		perror("execvp"); // execvp只有在出错时才会返回
//		exit(EXIT_FAILURE);
//	} else {
//		// 父进程
//		printf("[%d] %d\n", num_bg_processes + 1, pid); // 显示作业号和PID
//		add_bg_process(pid, argv[0]); // 添加到后台进程列表
//	}
//	return 0;
//}
//
//// 你可能还需要实现一个函数来检查并清理已经完成的后台进程
//void check_bg_processes() {
//	int status;
//	pid_t pid;
//	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
//		printf("Process %d finished\n", pid);
//		remove_bg_process(pid);
//	}
//}
