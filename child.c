//
// Created by XuZY on 2023/10/13.
//

#include "child.h"

void read_command() {
	// read command from terminal
	char *command;
	int c, index = 0;
	command = (char *)malloc(sizeof(char) * MAX_WORD_SIZE);
	printf(">: ");
	for (; (c = getchar()) != '\n' && c != EOF;) {
		command[index++] = (char)c;
	}
	command[index] = '\0';
	if (strcmp("exit\0", command) == 0) {
		printf("goodbye\n");
		_exit(EXIT_SUCCESS);
	} else {
		process_cmd(command);
		_exit(EXIT_SUCCESS);
	}
}

void process_cmd(char * cmd){
	char ** cmd_table = (char **)malloc(sizeof(char*) * 1024);
	for (int i = 0;i<1024;i++){
		cmd_table[i] = (char*)malloc(sizeof (char)*1024);
	}
	int idx = 0;
	while(isspace(cmd[idx])){
		idx++;
	}
	int col = 0,row = 0;
	unsigned long length_cmd = strlen(cmd);
	for (; idx < length_cmd; idx++) {
		if (cmd[idx] == ';') {
			cmd_table[row][col] = '\0';
			row++;
			col = 0;
		} else if (isspace(cmd[idx])) {
			cmd_table[row][col] = '\0';
			row++;
			col = 0;
		} else {
			cmd_table[row][col] = cmd[idx];
			col++;
		}
	}
	cmd_table[row][col] = '\0';
	for (int i = 0; i < row; ++i) {
		printf("%s \n",cmd_table[i]);
	}
	execvp(cmd_table,);
	printf("%s",cmd);
	sleep(1);
	for (int i = 0; i < 1024; ++i) {
		free(cmd_table[i]);
	}
	free(cmd_table);
	free(cmd);
	_exit(EXIT_SUCCESS);
}
