//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_

void preprocess(char *input,int length);

void split_space(char *cmd,char *args[]);
void split_semicolon(char *input);
void split_logop(char *input);

void split(char *input,int length);

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_PREPRO_INPUT_H_
