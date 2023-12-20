//
// Created by XuZY on 2023/11/25.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_

#define RESET_COLOR "\x1b[0m"
#define DIRECTORY_COLOR "\x1b[34m"
#define FILE_COLOR "\x1b[0m"
#define EXECUTABLE_COLOR "\x1b[32m"


void get_stat(char *file_path,int is_all,int rec);
int command_myls(char *path[],int length);
void list_files(char *the_path,int is_all,int Rec);
int test_search(char * options);


#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_MYSH_H_
