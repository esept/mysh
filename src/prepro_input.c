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
 * @brief Ignore space in the input string
 * @param cmd The input string
 * @return void
 */
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

/**
 * @brief Split the input by logical operators
 * @param input The input string
 * @return void
*/
void split_logop(char *input) {
    char *rest_line = input;
    char *cmd_seg;
    int last_status = 0;

    while ((cmd_seg = strtok_r(rest_line, "&|", &rest_line))) {
        char *tabcmd[SIZE];
        split_space(cmd_seg, tabcmd);

        if (strstr(cmd_seg, "&&") != NULL) {
            if (exec_cmd(tabcmd) != 0) {
                // On failure
                break;
            } else {
                // On success
                continue;
            }
        } else if (strstr(cmd_seg, "||") != NULL) {
            if (exec_cmd(tabcmd) == 0) {
                // On success
                cmd_seg = strtok_r(rest_line, "&|", &rest_line);
            } else {
                // On failure
                continue;
            }
        } else {
            // No logical operator
            int exec_status = exec_cmd(tabcmd);
            printf("exec_status: %d\n", exec_status);

            last_status = exec_status;
        }
    }
}

/**
 * @brief Preprocess the input string
 * @param input The input string
 * @param length The length of the input string
 * @return void
*/
void preprocess(char *input, int length) {
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	char *rest_line = input;
	char *cmd_seg;
	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
		split_logop(cmd_seg);
	}
}