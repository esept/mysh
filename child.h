//
// Created by XuZY on 2023/10/13.
//

#ifndef PRO_SE_MYSH__CHILD_H_
#define PRO_SE_MYSH__CHILD_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 1024
#define MAX_WORD 1024

void read_command();
void run_command(char *cmd);
void getin(char **list,char* orglist,int pos1,int pos2);
void process_cmd(char *cmd);
#endif //PRO_SE_MYSH__CHILD_H_
