//
// Created by XuZY on 2023/12/23.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_

#define MAXVAR 100

struct LocalVar{
	char var_name[50];
	char value[100];
};

struct LocalVar Vars[MAXVAR];
//int NBval = 0;

void init_variable();

int set_variable(int argc,char *argv[]);
int use_variable(int argc,char *argv[]);


int set_local_var(char *var_name,char *var_value);
char * get_local_var(char *val_name);
int unset_local_var(char *val_name);

int set_env_var(char *var_name,char *var_value);

#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_
