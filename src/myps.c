//
// Created by XuZY on 2023/12/2.
//

// unsed
//
//#include "../include/myps.h"
//#include "../include/main.h"
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <dirent.h>
//#include <string.h>
//#include <ctype.h>
//
//
//
//void show_status(const char *path){
//	FILE *fp = fopen(path,"r");
//	if (fp == NULL) getError("fopen NULL");
//	char line[SIZE];
//	while (fgets(line, sizeof(line),fp) != NULL){
//		if (strncmp(line,"Name:",5) == 0 || strncmp(line,"State:",6) == 0){
//			printf("%s",line);
//		}
//	}
//	fclose(fp);
//}
//
//
//void command_myps(){
//	DIR *dir = opendir("/proc");
//	if (dir == NULL) getError("open proc");
//	struct dirent *entry;
//	while ((entry = readdir(dir)) != NULL){
//		if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])){
//			char path[SIZE];
//			snprintf(path,sizeof(path),"/proc/%s/status",entry->d_name);
//
			printf("PID : %s\n",entry->d_name);

			printf("\n");
		}
	}
	closedir(dir);
}