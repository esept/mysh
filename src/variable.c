//
// Created by XuZY on 2023/12/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include <sys/ipc.h>
#include <sys/shm.h>


#include "../include/variable.h"
#include "../include/main.h"
#include "../include/exec.h"

int nbVal = 0;
int nbEnvVal = 0;
int shmid;

struct LocalVar Vars[MAXVAR];
SharedMemory *shared;

void init_variable() {
	int i;
	for (i = 0; i < MAXVAR; i++) {
		Vars[i].var_name[0] = '\0';
		Vars[i].value[0] = '\0';
	}
} // initial all local variable

char *replace_variable_references(char *value) {

	if (value[0] == '$') {
		char *actual_value = get_local_var(value + 1); 
		if (actual_value) {
			return actual_value;
		}
	}
	return value;
}

int set_variable(int argc, char *argv[]) {
	int rt_value = 0;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "set") == 0 && i + 1 < argc) {
			char *var_value;
			char *var_name = strtok_r(argv[i + 1], "=", &var_value); // split `=`
			if (var_name != NULL && var_value != NULL) {
				var_value = replace_variable_references(var_value); // have $
				rt_value = set_local_var(var_name, var_value);
			} else {
				perror("too few info");
				rt_value = -1;
			}
			break;
		} else if (strcmp(argv[i], "setenv") == 0 && i + 1 < argc) {
			char *var_value;
			char *var_name = strtok_r(argv[i + 1], "=", &var_value); // split `=`
			if (var_name != NULL && var_value != NULL) {
				rt_value = set_env_var(var_name, var_value);
			}else{
				perror("too few info");
				rt_value = -1;
			}
			break;
		} else if (strcmp(argv[i], "unset") == 0 && i + 1 < argc) {
			rt_value = unset_local_var(argv[i + 1]);

			if (rt_value != 0){
				fprintf(stderr,"NO such variable");
			}
			break;
		}else if (strcmp(argv[i], "unsetenv") == 0 && i + 1 < argc){
			rt_value = unset_env_var(argv[i + 1]);

			if (rt_value != 0){
				fprintf(stderr,"NO such variable");
			}
			break;
		}
	}
	return rt_value;
} // set/setenv/unset variable commande


int use_variable(int argc, char *argv[]) {
	int rt_value,i,j,k;
	for (i = 0; i < argc; i++) {
		char *param = argv[i];
		char new_param[1024] = "";
		j = 0, k = 0;

		while (param[j] != '\0') {
			if (param[j] == '$') {
				// get name
				j++;
				char var_name[100];
				int var_len = 0;
				while (param[j] != '\0' && isalnum(param[j])) {
					var_name[var_len++] = param[j++];
				}
				var_name[var_len] = '\0';

				// get variable
				char *value = get_local_var(var_name);
				if (value == NULL) {
					value = get_env_var(var_name);
				}
				if (value == NULL) {
					fprintf(stderr, "Error: Variable '%s' not set.\n", var_name);
					return 0;
				}

				strcpy(new_param + k, value);
				k += strlen(value);
			} else {
				new_param[k++] = param[j++];
			}
		}
		new_param[k] = '\0';
		argv[i] = strdup(new_param);
	}
	rt_value = exec_cmd(argv); // exec commande
	return rt_value;
} // get $variable commande


int set_local_var(char *var_name, char *var_value) {
	int i ;
	for (i = 0; i < nbVal; i++) {
		if (strcmp(Vars[i].var_name, var_name) == 0) {
//			printf("change %s from %s to %s",Vars[i].var_name,Vars[i].value,var_value);
			strcpy(Vars[i].value, var_value);
			return 0;
		}
	}
	if (nbVal >= MAXVAR) {
		perror("Too many variables");
		return -1;
	}
	strcpy(Vars[nbVal].var_name, var_name);
	strcpy(Vars[nbVal].value, var_value);
	nbVal++;
	return 0;
} // set local variable commande


char * get_local_var(char *var_name) {
	for (int i = 0; i < nbVal; i++) {
		if (strcmp(Vars[i].var_name, var_name) == 0) {
			return Vars[i].value;
		}
	}
	return NULL;
} // get local variable commande


int unset_local_var(char *var_name) {
	for (int i = 0; i < nbVal; i++) {
		if (strcmp(Vars[i].var_name, var_name) == 0) {
			// found variable and move forward each variable
			for (int j = i; j < nbVal - 1; j++) {
				strcpy(Vars[j].var_name, Vars[j + 1].var_name);
				strcpy(Vars[j].value, Vars[j + 1].value);
			}
			// set final variable as init state
			Vars[nbVal - 1].var_name[0] = '\0';
			Vars[nbVal - 1].value[0] = '\0';
			nbVal--;
			return 0;
		}
	}
	return -1; // 如果找不到变量
} // unset local variable commande

void init_sharedMemeory(){
	key_t key = ftok("/tmp",'R'); // may need change
	if (key == -1){
		perror("ftok");
		return;
	}
	shmid = shmget(key, sizeof(EnvVar) * MAXENVVAR ,0640|IPC_CREAT);
	if (shmid == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	shared = (SharedMemory*) shmat(shmid, NULL, 0);
	if (shared == (void*)-1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	pthread_rwlock_init(&shared->lock, NULL);
} // init shared memory

int set_env_var(char *var_name, char *var_value) {
	int i;
	pthread_rwlock_wrlock(&shared->lock);
	for (i = 0; i < MAXENVVAR; ++i) {
		if (strcmp(shared->eVars[i].var_name,var_name) == 0){
			strcpy(shared->eVars[i].value,var_value);
			pthread_rwlock_unlock(&shared->lock); // protect shared memory

			return 0;
		}
	}
	for (i = 0; i < MAXENVVAR; ++i) {
		if (shared->eVars[i].var_name[0] == '\0') {
			strcpy(shared->eVars[i].var_name, var_name);
			strcpy(shared->eVars[i].value, var_value);
			pthread_rwlock_unlock(&shared->lock);

			return 0;
		}
	}
	fprintf(stderr,"NO more space avaliable");
	pthread_rwlock_unlock(&shared->lock);
	return -1;
} // set env variable

int unset_env_var(char *var_name){
	int i;
	for (i = 0; i < MAXENVVAR; ++i) {
		if (strcmp(shared->eVars[i].var_name,var_name) == 0){
			shared->eVars[i].var_name[0] = '\0';
			shared->eVars[i].value[0] = '\0';
			return 0;
		}
	}
	return -1;
} // unset env variable

char *get_env_var(char *var_name){
	int i;
	pthread_rwlock_rdlock(&shared->lock);
	for (i = 0; i < MAXENVVAR; ++i) {
		if (strcmp(shared->eVars[i].var_name,var_name) == 0){
			pthread_rwlock_unlock(&shared->lock);
			return shared->eVars[i].value;
		}
	}
	pthread_rwlock_unlock(&shared->lock);
	return NULL;
} // get env variable

void clean_env_variable(){
	if (shared->eVars != NULL){
		shmdt(shared->eVars);
		shmctl(shmid,IPC_RMID,NULL);
	}
} // delete shared memory

void clean_local_variable(){
	int i;
	for (i = 0; i < MAXVAR; i++) {
		Vars[i].var_name[0] = '\0';
		Vars[i].value[0] = '\0';
	}
} // delete local memory
