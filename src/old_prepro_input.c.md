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
/**
void preprocess(char *input,int length){
	if (length > 0 && input[length - 1] == '\n'){
		input[length - 1] = '\0';
	}
	char *rest_line = input;
	char * cmd_seg;
	split_semicolon(input);
//	while ((cmd_seg = strtok_r(rest_line,";",&rest_line))){
//
//		char *tabcmd[SIZE],*token,*rest_cmd = cmd_seg;
//		int i = 0;
//		while ((token = strtok_r(rest_cmd," ",&rest_cmd)) != NULL && i < SIZE){
//			tabcmd[i++] = token;
//		}
//		tabcmd[i] = NULL;
//		if (tabcmd[0] != NULL){
//			if (strcmp(tabcmd[0],"cd") == 0){
//				change_dict(tabcmd[1]);
//				continue;
//			} else if(strcmp(tabcmd[0],"exit") == 0){
//				exit(EXIT_SUCCESS);
//			}
//		}
//		exec_cmd(tabcmd);
//	}

}

void split_space(char *cmd,char *args[]){
	int i = 0;
	char *token;
	while ((token = strtok_r(cmd," ",&cmd)) != NULL && i < SIZE){
		args[i++] = token;
	}
	args[i] = NULL;
}

void split_semicolon(char *input){
	char *rest_line = input;
	char *cmd_seg;
	while((cmd_seg = strtok_r(rest_line,";",&rest_line))){
		split_logop(cmd_seg);
	}
}

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

void preprocess(char *input, int length) {
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	char *rest_line = input;
	char *cmd_seg;
	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
		char *tabcmd[SIZE], *token, *rest_cmd = cmd_seg;
		int i = 0;

		while ((token = strtok_r(rest_cmd, " ", &rest_cmd)) != NULL && i < SIZE) {
			tabcmd[i++] = token;
		}
		tabcmd[i] = NULL;
		if (tabcmd[0] != NULL) {
			if (strcmp(tabcmd[0], "cd") == 0) {
				command_cd(tabcmd[1]);
				continue;
			} else if (strcmp(tabcmd[0],"myls") == 0) {
				command_myls(tabcmd,i);
				continue;
			}
			else if (strcmp(tabcmd[0], "exit") == 0) {
				exit(EXIT_SUCCESS);
			}else {
				exec_cmd(tabcmd);
			}
		}
//		exec_cmd(tabcmd);
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