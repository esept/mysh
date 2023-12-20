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

/**
void split_logop(char *input){
	char *rest_line = input;
	char *cmd_seg;
	int last_status = 0;
	int exec_status = 1;
	while ((cmd_seg = strtok_r(rest_line,"&&",&rest_line))){
		char *tabcmd[SIZE];
		split_space(rest_line,tabcmd);
		if ((strstr(cmd_seg,"&&") != NULL && last_status == 0) ||
				(strstr(cmd_seg,"||") != NULL && last_status != 0) ||
				(strstr(cmd_seg,"&&") != NULL && strstr(cmd_seg,"||") == NULL )){
			if (strcmp(tabcmd[0],"cd") == 0){
				last_status = change_dict(tabcmd[1]);
				continue;
			} else if(strcmp(tabcmd[0],"exit")){
				exit(EXIT_SUCCESS);
			}else{
				last_status = exec_cmd(tabcmd);
				exec_status = 1;
			}
		} else{
			exec_status = 0;
		}
	}
}
 **/

//void preprocess(char *input, int length) {
//	if (length > 0 && input[length - 1] == '\n') {
//		input[length - 1] = '\0';
//	}
//	char *rest_line = input;
//	char *cmd_seg;
//	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
//		char *tabcmd[SIZE], *token, *rest_cmd = cmd_seg;
//		int i = 0;
//
//		while ((token = strtok_r(rest_cmd, " ", &rest_cmd)) != NULL && i < SIZE) {
//			tabcmd[i++] = token;
//		}
//		tabcmd[i] = NULL;
//		if (tabcmd[0] != NULL) {
//			if (strcmp(tabcmd[0], "cd") == 0) {
//				command_cd(tabcmd[1]);
//				continue;
//			} else if (strcmp(tabcmd[0], "myps") == 0) {
//				command_myps();
//				continue;
//			} else if (strcmp(tabcmd[0], "myls") == 0) {
//				command_myls(tabcmd, i);
//				continue;
//			} else if (strcmp(tabcmd[0], "exit") == 0) {
//				exit(EXIT_SUCCESS);
//			} else {
//				exec_cmd(tabcmd);
//			}
//		}
//	}
//
//}

//void preprocess(char *input,int length){
//	if(length > 0 && input[length -1] == '\n'){
//		input[length - 1] = '\0';
//	}
//	char *rest_input = input;
//	char *command_segment;
//	while ((command_segment = strtok_r(rest_input,";",&rest_input))){
//		char *rest_cmd = command_segment;
//		char *cmd_part ;
//		int last_status = 0;
//		int execute_next = 1;
//		char *next = strstr(rest_cmd, "&&");
//		char *next_or = strstr(rest_cmd,"||");
//		char *op = "&&";
//		if (next == NULL || (next_or != NULL && next_or < next)) {
//			op = "||";
//		} // 显示最靠前的逻辑符号
//		while ((cmd_part = strtok_r(rest_cmd,op,&rest_cmd))){
//			char *tabcmd[SIZE],*token,*rrcmd = cmd_part;
//			int i = 0;
//
//			while ((token = strtok_r(rrcmd," ",&rrcmd)) != NULL && i < SIZE){
//				tabcmd[i++] = token;
//			}
//			tabcmd[i] = NULL;
//			if (execute_next){
//				if (strcmp(tabcmd[0],"cd") == 0){
//					command_cd(tabcmd[1]);
//					last_status = 0;
//				} else if(strcmp(tabcmd[0],"exit") == 0){
//					exit(EXIT_SUCCESS);
//				} else if (strcmp(tabcmd[0],"myls") == 0){
//					command_myls(tabcmd,i);
//					last_status = 0;
//					continue;
//				}else{
//					exec_cmd(tabcmd);
//				}
//			}
//			if (*rest_cmd == '&'){
//				execute_next = (last_status == 0);
//			}else if (*rest_cmd == '|'){
//				execute_next = (last_status != 0);
//			}
//
//			if(*rest_cmd != '\0'){
//				rest_cmd++;
//			}
//
//		}
//	}
//}

void preprocess(char *input, int length) {
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	int last_command_status = 0;
	char *rest_input = input;
	char *command_segment;

	while ((command_segment = strtok_r(rest_input, ";", &rest_input))) {
		char *rest_cmd = command_segment;
		int last_status = 0;
		int execute_next = 1;
		if (rest_cmd == NULL){
			break;
		}
		while (*rest_cmd != NULL && *rest_cmd != '\0' ) {
			char *next = strstr(rest_cmd, "&&");
			char *next_or = strstr(rest_cmd, "||");
			char *op_end;

			if (next == NULL || (next_or != NULL && next_or < next)) {
				next = next_or;
				op_end = next ? next + 2 : NULL; // 对于 '||'
			} else {
				op_end = next + 2; // 对于 '&&'
			}

			if (next) {
				*next = '\0'; // 切断当前命令
			}
			if (strstr(rest_cmd,"|")){
				if (execute_next){
					exec_pipe(rest_cmd);
				}
			}else {

				char *tabcmd[SIZE], *token;
				int i = 0;

				while ((token = strtok_r(rest_cmd, " ", &rest_cmd)) != NULL && i < SIZE) {
					tabcmd[i++] = token;
				}
				tabcmd[i] = NULL;

				if (execute_next) {
					if (strcmp(tabcmd[0], "cd") == 0) {
						last_status = command_cd(tabcmd[1]);
					} else if (strcmp(tabcmd[0], "exit") == 0) {
						exit(EXIT_SUCCESS);
					} else if (strcmp(tabcmd[0], "myls") == 0) {
						last_status = command_myls(tabcmd, i);
//						continue;
					}else if (strcmp(tabcmd[0],"status") == 0){
						printf("Last command status = %d\n",last_command_status);
					}else {
						last_status = exec_cmd(tabcmd);
					}
				}
			}
			if (next) {
				execute_next = (last_status == 0 && strcmp(next, "&") == 0) || (last_status != 0 && strcmp(next, "|") == 0);
				rest_cmd = op_end;
			} else {
				break; // 没有更多的逻辑运算符
			}
		}
		last_command_status = last_status;
	}
}


void split_space(char *cmd, char *args[]) {
	int i = 0;
	char *token;
	while ((token = strtok_r(cmd, " ", &cmd)) != NULL && i < SIZE) {
		args[i++] = token;
	}
	args[i] = NULL;
}

void split_semicolon(char *input) {
	char *rest_line = input;
	char *cmd_seg;
	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
		char *tabcmd[SIZE];
		split_space(cmd_seg, tabcmd);
		if (strcmp(tabcmd[0], "cd") == 0) {
			command_cd(tabcmd[1]);
			continue;
		} else if (strcmp(tabcmd[0], "exit") == 0) {
			exit(EXIT_SUCCESS);
		}
		exec_cmd(tabcmd);
	}
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
		char *next_or = strstr(rest,"||");
		if (next == NULL || (next_or != NULL && next_or < next)) {
			next = next_or;
		}
		int pos_rest = strlen(rest);
		int pos_next = strlen(next);

		

	}
}