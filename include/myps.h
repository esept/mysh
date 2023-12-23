//
// Created by XuZY on 2023/12/2.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_DISPLAY_LEN 12
#define MAX_DISPLAY_LEN_FOR_FLOAT_AND_UNSIGNED 7

struct ProcessInfo {
    char user[MAX_LINE_LEN];
    int pid;
    float cpu;
    float mem;
    unsigned long vsz;
    unsigned long rss;
    char tty[MAX_LINE_LEN];
    char stat[MAX_LINE_LEN];
    char start[MAX_LINE_LEN];
    char time[MAX_LINE_LEN];
    char command[MAX_LINE_LEN];
};

void printHeader();
void printProcessInfo(struct ProcessInfo *info);
void command_myps();

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_
