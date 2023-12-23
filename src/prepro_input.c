//
// Created by XuZY on 2023/11/25.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/prepro_input.h"
#include "../include/main.h"
#include "../include/exec.h"
#include "../include/myls.h"
#include "../include/myps.h"

int cmdop(char *input[], int a, int len) {
	for (int i = a + 1; i < len; ++i) {
		if (strcmp(input[i], "&&") == 0 || strcmp(input[i], "||") == 0) {
			return i;
		}
	}
	return 0;
}

void preprocess(char *input, int length) {
	int i;
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	int argc, execute_next = 1, lop = 0;
	char *argv[CMDLEN];
	i = 0;
	char *rest_cmd = input;
	char *cmd_seg;
	int logop[CMDLEN];
	int posop;
	while ((cmd_seg = strtok_r(rest_cmd, ";", &rest_cmd))) {
		argc = split_space(cmd_seg, argv);
		posop = cmdop(argv, 0, argc);
		while (posop != 0) {
			logop[i++] = posop;
			posop = cmdop(argv, posop, argc);
		}

		int last_status = 0; // 上一个命令的返回状态
		logop[i++] = posop;
		if (i != 0) { // 如果有逻辑操作
			for (int j = 0; j < i; j++) {
				if (j == 0 || (strcmp(argv[logop[j-1]],"&&") == 0 && last_status == 0) || ( strcmp(argv[logop[j-1]],"||") == 0 && last_status != 0)) {
					int start = (j == 0) ? 0 : logop[j-1] + 1;
					int end = (j < i - 1) ? logop[j] : argc;
					last_status = process(argc, argv, start, end);
					status = last_status;
				}
			}
		} else {
			last_status = process(argc, argv, 0, argc);
			status = last_status;
		}
	}
}

int process(int argc, char *argv[], int start, int end) {
	char *cmd_argv[CMDLEN];
	int cmd_argc = 0, return_status ,i;
	pid_t pid;
	for (i = start; i < end && i < argc; i++) {
		if (strcmp(argv[i], "&&") != 0 && strcmp(argv[i], "||") != 0) {
			cmd_argv[cmd_argc++] = argv[i];
		}
	}
	cmd_argv[cmd_argc] = NULL;

	for (i = 0; i < ; ++i) {

	}




	if (strcmp(cmd_argv[0],"cd") == 0){
		return_status = command_cd(cmd_argv[1]);
	}else if(strcmp(cmd_argv[0],"myls") == 0){
		return_status = command_myls(cmd_argv,cmd_argc);
	}else if(strcmp(cmd_argv[0],"exit") == 0){
		exit(EXIT_SUCCESS);
	} else if(strcmp(cmd_argv[0],"status") == 0){
		printf("last status = %d",status);
		return_status = 1;
	}else{
		return_status = exec_cmd(cmd_argv);
	}
	return return_status;
}



//char **split_tab(char *tabs[], char *tabrest[], int a, int b) {
//	int x = b - a + 1;
////	char *tabrest[x];
//	tabrest = (char **)malloc(sizeof(char *) * x);
//	for (int i = a; i < b + 1; ++i) {
//		tabrest[i] = (char *)malloc(sizeof(char) * CMDLEN);
//		strcpy(tabrest[i - a], tabs[i]);
//	}
//	return tabrest;
//}

int split_space(char *cmd, char *args[]) {
	int i = 0, j = 0;
	char *token;
	while ((token = strtok_r(cmd, " ", &cmd)) != NULL && i < SIZE) {

		args[i++] = token;
		j++;
	}
	args[i] = NULL;
	return j;
}

void split_semicolon(char *input) {
	char *rest_line = input;
	char *cmd_seg;
	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
		char *tabcmd[SIZE];
//		split_space(cmd_seg, tabcmd);
		if (strcmp(tabcmd[0], "cd") == 0) {
			command_cd(tabcmd[1]);
			continue;
		} else if (strcmp(tabcmd[0], "exit") == 0) {
			exit(EXIT_SUCCESS);
		}
		printf("%s\n", cmd_seg);
//		exec_cmd(tabcmd);
	}
	printf("%s", cmd_seg);
}

void split(char *input, int length) {
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	char *tabcmds[SIZE];
	int cmd_count = 0;
	// int last_status = 0;
	// int execute_next = 1;
	char *token = strtok(input, ";");
	while (token != NULL && cmd_count < SIZE) {
		tabcmds[cmd_count++] = token;
		token = strtok(NULL, ";");
	}
	for (int i = 0; i < cmd_count; ++i) {
		printf("%s\n", tabcmds[i]);
	}
	for (int i = 0; i < cmd_count; ++i) {
		char *rest = tabcmds[i];
		char *next = strstr(rest, "&&");
		char *next_or = strstr(rest, "||");
		if (next == NULL || (next_or != NULL && next_or < next)) {
			next = next_or;
		}
		int pos_rest = strlen(rest);
		int pos_next = strlen(next);

	}
}