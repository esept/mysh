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

typedef struct job {
    int job_number;
    pid_t pid;
    char* status;
    char* command;
} job;

extern job bg_jobs[MAX_BG_PROCESSES];
extern int last_job_index;

void add_job(pid_t pid, char* command);
void update_jobs();
int print_jobs();

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MAIN_H_
