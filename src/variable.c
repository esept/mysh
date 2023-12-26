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
/**
* 6 Les variables
Pour être un interpréteur de commandes qui se respecte, votre shell doit intégrer la prise en charge de variables. Deux types de variables sont à gérer : les variables locales et d’environnement.
6.1 Les variables d’environnment
Les variables d’environnement sont stockées dans une zone de mémoire partagée qui est créée par le premier exemplaire de votre shell qui tourne à un instant donné et est initialisée avec toutes les variables définies dans le tableau envp (int main(int argc,char *argv[],char *envp[])). Le dernier exemplaire de votre shell qui tourne à un instant donné doit détruire cet espace partagé avant de mourir.
Cette zone de mémoire partagée est à gérer comme une mémoire par subdivison.
La zone de mémoire partagée est une zone de mémoire à accès concurrents avec plusieurs lectures simul- tanées possibles mais 1 seule écriture possible à un instant donné. L’implémentation demandée doit donner la priorité à l’écriture.

6.2 Les variables locales
Elles sont locales à votre shell, c’est-à-dire que dex exemplaires de votre shell peuvent avoir un même nom de variable avec une valeur différente dans chaque shell.
6.3 Utilisation des variables
Pour accéder à une variable sur la une ligne de commande, on utilisera le symbole $ devant son nom.
Si une variable de nom nnn existe à la fois en locale en environnement, la variable locale est prioritaire. La
valeur de cette dernière sera donc utilisée.
Pour définir la valeur d’une variable on utlisera respectivement les commandes internes set et setenv pour les variables locales et d’environnment. Ces commandes admettent un paramètre de la forme : var=valeur où var représente la variable à définir et valeur la valeur à lui associer. Si ces commandes sont lancées sans pa- ramètre, elles afficheront les respectivement les valeurs de l’ensemble des variables locales et d’environnment.
Les commandes internes unset et unsetenv admettent obligatoirement un paramètre qui est le nom de la variable à supprimer et ont pour objectif de supprimer respectivement une variable locale ou d’environnement.
Exemple 8
∼>set a=foo ∼>echo $a
foo
∼>setenv b=duc ∼>mysh
∼>echo $b
duc
∼>set a=bar ∼>set b=tmp ∼>echo $b
tmp
∼>setenv b=$a ∼>exit
∼>echo $b+$a bar+foo ∼>unset $a
*/

int nbVal = 0;
int nbEnvVal = 0;
int shmid;

void init_variable() {
	int i;
	for (i = 0; i < MAXVAR; i++) {
		Vars[i].var_name[0] = '\0';
		Vars[i].value[0] = '\0';
	}
} // initial all local variable


int set_variable(int argc, char *argv[]) {
	int rt_value = 0;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "set") == 0 && i + 1 < argc) {
			char *var_value;
			char *var_name = strtok_r(argv[i + 1], "=", &var_value);
			if (var_name != NULL && var_value != NULL) {
				rt_value = set_local_var(var_name, var_value);
			} else{
				perror("too few info");
				rt_value = -1;
			}
			break;
		} else if (strcmp(argv[i], "setenv") == 0 && i + 1 < argc) {
			char *var_value;
			char *var_name = strtok_r(argv[i + 1], "=", &var_value);
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
			// 找到变量，将后面的变量向前移动一位
			for (int j = i; j < nbVal - 1; j++) {
				strcpy(Vars[j].var_name, Vars[j + 1].var_name);
				strcpy(Vars[j].value, Vars[j + 1].value);
			}
			// 更新最后一个变量为初始状态
			Vars[nbVal - 1].var_name[0] = '\0';
			Vars[nbVal - 1].value[0] = '\0';
			nbVal--;
			return 0;
		}
	}
//	perror("NO SUCH variable");
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
}

int set_env_var(char *var_name, char *var_value) {
	int i;
	pthread_rwlock_wrlock(&shared->lock);
	for (i = 0; i < MAXENVVAR; ++i) {
		if (strcmp(shared->eVars[i].var_name,var_name) == 0){
			strcpy(shared->eVars[i].value,var_value);
//			printf("set env var %s = %s",var_name,var_name);
			pthread_rwlock_unlock(&shared->lock);

			return 0;
		}
	}
	for (i = 0; i < MAXENVVAR; ++i) {
		if (shared->eVars[i].var_name[0] == '\0') {
			strcpy(shared->eVars[i].var_name, var_name);
			strcpy(shared->eVars[i].value, var_value);
			pthread_rwlock_unlock(&shared->lock);

//			printf("set env var %s = %s",var_name,var_name);
			return 0;
		}
	}
	fprintf(stderr,"NO more space avaliable");
	pthread_rwlock_unlock(&shared->lock);
	return -1;
}

int unset_env_var(char *var_name){
	int i;
	for (i = 0; i < MAXENVVAR; ++i) {
		if (strcmp(shared->eVars[i].var_name,var_name) == 0){
			shared->eVars[i].var_name[0] = '\0';
			shared->eVars[i].value[0] = '\0';
			return 0;
		}
	}
//	fprintf(stderr,"NO such variable");
	return -1;
}

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
}

void clean_env_variable(){
	if (shared->eVars != NULL){
		shmdt(shared->eVars);
		shmctl(shmid,IPC_RMID,NULL);
	}
}

void clean_local_variable(){
	int i;
	for (i = 0; i < MAXVAR; i++) {
		Vars[i].var_name[0] = '\0';
		Vars[i].value[0] = '\0';
	}
}