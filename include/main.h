//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_

#include <unistd.h>
#include <sys/types.h>

#define getError(x) perror(x),exit(EXIT_FAILURE)

#define ERR -1
#define CMDLEN 4096

#define SIZE 1024

extern pid_t globalPID;

#define MAX_BG_PROCESSES 100
extern pid_t bg_processes[MAX_BG_PROCESSES];
extern int last_bg_process_index;

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_
