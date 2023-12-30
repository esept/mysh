//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_EXEC_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_EXEC_H_

int exec_cmd(char *cmd[]);
int command_cd(char *path);
void exec_pipe(char *cmd);
int cmd_pipe(char *argv[],int argc);
int cmd_pipe2(char *argv[],int argc);

int cmd_redi(char *argv[],int argc);

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_EXEC_H_
