//
// Created by XuZY on 2023/12/23.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_

#include <pthread.h>

#define MAXVAR 100
#define MAXENVVAR 20

struct LocalVar{
	char var_name[50];
	char value[100];
};

typedef struct EnvVar{
	char var_name[50];
	char value[100];
}EnvVar;

struct LocalVar Vars[MAXVAR];
//EnvVar *eVars;

typedef struct {
	pthread_rwlock_t lock;
	EnvVar eVars[MAXENVVAR];
} SharedMemory;

SharedMemory *shared;

void init_variable();

int set_variable(int argc,char *argv[]);
int use_variable(int argc,char *argv[]);


int set_local_var(char *var_name,char *var_value);
char * get_local_var(char *val_name);
int unset_local_var(char *val_name);

int set_env_var(char *var_name,char *var_value);

void init_sharedMemeory();
void init_env_variable();
int unset_env_var(char *var_name);
char *get_env_var(char *var_name);
void clean_env_variable();
void clean_local_variable();
#endif //EDU_1S_C_PROJECT_MYSHELL_SRC_VARIABLE_H_
