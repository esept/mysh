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
#include "../include/variable.h"

int getop(char **input, int a, int len) {
	for (int i = a + 1; i < len; ++i) {
		if (strcmp(input[i], "&&") == 0 || strcmp(input[i], "||") == 0) {
			return i;
		}
	}
	return 0;
} // get position of logic operators


void preprocess(char *input, int length) {
	int i;
	if (length > 0 && input[length - 1] == '\n') {
		input[length - 1] = '\0';
	}
	int argc, execute_next = 1, lop = 0;
	char *argv[CMDLEN];
	char *rest_cmd = input;
	char *cmd_seg;
	int logop[CMDLEN];
	int posop;
	int last_status = 0;
	int status = 0;

	while ((cmd_seg = strtok_r(rest_cmd, ";", &rest_cmd))) { // split semicolon
		i = 0;
		argc = split_space(cmd_seg, argv); // split space

		// get logic operator position
		posop = getop(argv, 0, argc);
		while (posop != 0 && i < CMDLEN - 1) {
			logop[i++] = posop;
			posop = getop(argv, posop, argc);
		}
		logop[i] = argc;

		// check have logop or not
		if (i > 0) {
			// have logop
			int start = 0;
			for (int j = 0; j <= i; j++) {
				int end = (j < i) ? logop[j] : argc;
				if (j == 0 ||
						(strcmp(argv[logop[j - 1]], "&&") == 0 && last_status == 0) ||
						(strcmp(argv[logop[j - 1]], "||") == 0 && last_status != 0)) { // split logic operator

					last_status = process(argc, argv, start, end); // exec command with numbre of position
					status = last_status;
				}
				start = end + 1;
			}
		} else {
			// dont have logop
			last_status = process(argc, argv, 0, argc);
			status = last_status;
		}
	}
} // preprocess command


int process(int argc, char *argv[], int start, int end) {
	char *cmd_argv[CMDLEN];
	int cmd_argc = 0;
	int return_status = 0,i;

	// get the real command
	for (i = start; i < end && i < argc; i++) {
		if (strcmp(argv[i], "&&") != 0 && strcmp(argv[i], "||") != 0) {
			cmd_argv[cmd_argc++] = argv[i];
		}
	}
	cmd_argv[cmd_argc] = NULL;


	// if command use pipe
	for (i = 0; i < cmd_argc; ++i) {
		if (strcmp(cmd_argv[i],"|") == 0){ // pipe |
			printf("pipe");
			return_status = cmd_pipe2(cmd_argv,cmd_argc);
			return return_status;
		}
	}


	// if command set/unset variables
	char *variable_tab[] = {
			"set","setenv","unset","unsetenv"
	};
	for (i = 0; i < cmd_argc; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (strcmp(cmd_argv[i],variable_tab[j]) == 0){
				return_status = set_variable(cmd_argc,cmd_argv);
				return return_status;
			}
		}
	}


	// if command use variable
	for (i = 0;  i<cmd_argc ; i++) {
		for (int j = 0; j < strlen(cmd_argv[i]); ++j) {
			if ('$' == cmd_argv[i][j]){
				return_status = use_variable(cmd_argc,cmd_argv);
				return return_status;
			}
		}
	}

	// if command use redirection
	char *redirect[] = {
			">",">>","2>","2>>",">&",">&>","<"
	};

	for (i = 0; i < cmd_argc; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (strcmp(cmd_argv[i],redirect[j]) == 0){
				return_status = cmd_redi(cmd_argv,cmd_argc);
				return return_status;
			}
		}
	}

	// if command use builtin command
	if (strcmp(cmd_argv[0],"cd") == 0){ // changer direct
		return_status = command_cd(cmd_argv[1]);
	}else if(strcmp(cmd_argv[0],"myls") == 0){ // ls -l
		return_status = command_myls(cmd_argv,cmd_argc);
	}else if(strcmp(cmd_argv[0],"exit") == 0){ // exit
		clean_env_variable();
		clean_local_variable();
		exit(EXIT_SUCCESS);
	} else if(strcmp(cmd_argv[0],"status") == 0){ // show final status
		printf("last status = %d",status);
		return_status = 1;
	}else{ // run command
		return_status = exec_cmd(cmd_argv);
	}
	return return_status;
}



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


// split space

//void split_semicolon(char *input) {
//	char *rest_line = input;
//	char *cmd_seg;
//	while ((cmd_seg = strtok_r(rest_line, ";", &rest_line))) {
//		char *tabcmd[SIZE];
////		split_space(cmd_seg, tabcmd);
//		if (strcmp(tabcmd[0], "cd") == 0) {
//			command_cd(tabcmd[1]);
//			continue;
//		} else if (strcmp(tabcmd[0], "exit") == 0) {
//			exit(EXIT_SUCCESS);
//		}
//		printf("%s\n", cmd_seg);
//	}
//	printf("%s", cmd_seg);
//}

//void split(char *input, int length) {
//	if (length > 0 && input[length - 1] == '\n') {
//		input[length - 1] = '\0';
//	}
//	char *tabcmds[SIZE];
//	int cmd_count = 0;
//	// int last_status = 0;
//	// int execute_next = 1;
//	char *token = strtok(input, ";");
//	while (token != NULL && cmd_count < SIZE) {
//		tabcmds[cmd_count++] = token;
//		token = strtok(NULL, ";");
//	}
//	for (int i = 0; i < cmd_count; ++i) {
//		printf("%s\n", tabcmds[i]);
//	}
//	for (int i = 0; i < cmd_count; ++i) {
//		char *rest = tabcmds[i];
//		char *next = strstr(rest, "&&");
//		char *next_or = strstr(rest, "||");
//		if (next == NULL || (next_or != NULL && next_or < next)) {
//			next = next_or;
//		}
//		int pos_rest = strlen(rest);
//		int pos_next = strlen(next);
//
//	}
//}


int is_builtin_command(char *cmd) {
//	printf("check builtin\n\n");
	const char *builtin_commands[] = {"cd", "exit", "myls", "status", NULL};

	for (int i = 0; builtin_commands[i] != NULL; i++) {
		if (strcmp(cmd, builtin_commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
} // if use builtin command

int execute_builtin_command(char *argv[], int pipefds[], int cmd_index, int num_pipes) {
	if (strcmp(argv[0], "cd") == 0) {
		return command_cd(argv[1]);
	} else if (strcmp(argv[0], "exit") == 0) {
		exit(EXIT_SUCCESS);
	} else if (strcmp(argv[0], "myls") == 0) {
		return command_myls(argv, cmd_index);
	} else if (strcmp(argv[0], "status") == 0) {
		printf("last status = %d\n", status);
		return 1;
	}

	return 0;
} // exec builtin command




