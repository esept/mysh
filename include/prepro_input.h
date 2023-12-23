//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_

int status;


int getop(char **input, int a, int len);
void preprocess(char *input,int length);
int process(int argc,char *argv[],int start,int end);
char ** split_tab(char *tabs[],char *tabrest[],int a,int b);

int split_space(char *cmd,char *args[]);
void split_semicolon(char *input);
void split_logop(char *input);

void split(char *input,int length);






int is_builtin_command(char *cmd);
execute_builtin_command(char *argv[], int pipefds[], int cmd_index, int num_pipes);

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_
