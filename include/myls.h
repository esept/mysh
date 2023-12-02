//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_

void get_stat(char *file_path,int is_all,int rec);
void command_myls(char *path[],int length);
void list_files(char *the_path,int is_all,int Rec);
int test_search(char * options);

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_
