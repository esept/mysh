//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_

#include <unistd.h>

#define getError(x) perror(x),exit(EXIT_FAILURE)

#define ERR -1
#define CMDLEN 4096

#define SIZE 1024

#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

extern pid_t globalPID;

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_
