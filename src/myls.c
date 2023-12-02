//
// Created by XuZY on 2023/11/25.
//

#include "../include/myls.h"
#include "../include/main.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void command_myls(char *path[],int length){
	int all = 0,Rec = 0,i,res;
//	for(i=0;i<length;i++){
//		printf("%s\n",path[i]);
//	}
	if (length == 1){
		list_files("./",all,Rec);
	}else	{
		for(i = 1;i<length;i++){
			res = test_search(path[i]);
			printf("res = %d",res);
			if (res == 0){
//				options[i-1] = (char *)malloc(sizeof(char)* strlen(argv[i]));
				all = 1;
			}
			if (res == 1){
				all = 1;
			}else{
				all = 1;
				Rec = 1;
			}
		}
		printf("rec = %d, all = %d\n",Rec,all);
//		for(i = 1;i<length;i++){
//
//		}
	}
}



void get_stat(char *file_path,int is_all,int rec) {
	int rt_stat;
	struct stat mystat;
	rt_stat = lstat(file_path, &mystat);
	if (rt_stat < 0){
		printf("%s",file_path);
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	char filemod[11];
	mode_t mod = (&mystat)->st_mode;

	if (rec == 1 && S_ISDIR(mod)){
		char newpath[strlen(file_path)+2];
		strcpy(newpath,file_path);
		strcat(newpath,"/");
//		strcat(newpath,"\0");
		printf("newpath = %s",newpath);
		list_files(newpath,is_all,rec);
	}

	filemod[0] = S_ISDIR(mod) ? 'd':'-';
	filemod[1] = (mod & S_IRUSR) ? 'r' : '-';
	filemod[2] = (mod & S_IWUSR) ? 'w' : '-';
	filemod[3] = (mod & S_IXUSR) ? 'x' : '-';
	filemod[4] = (mod & S_IRGRP) ? 'r' : '-';
	filemod[5] = (mod & S_IWGRP) ? 'w' : '-';
	filemod[6] = (mod & S_IXGRP) ? 'x' : '-';
	filemod[7] = (mod & S_IROTH) ? 'r' : '-';
	filemod[8] = (mod & S_IWOTH) ? 'w' : '-';
	filemod[9] = (mod & S_IXGRP) ? 'x' : '-';
	filemod[10] = '\0';

//	if (rt_stat == -1) {
//		perror("lstat");
//	}
	printf("%s",filemod);

	struct passwd *pw = getpwuid((&mystat) -> st_uid);
	struct group *gr = getgrgid((&mystat) -> st_gid);

	char datestring[256];
	struct tm *tm;
	tm = localtime(&(&mystat) -> st_mtime);
	// strftime(datestring,sizeof(datestring),"%b %d %H:%M",tm);
	strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);

	printf(" %lu \t%s \t%s \t%lld \t%s", (unsigned long)(&mystat)->st_nlink, pw->pw_name, gr->gr_name, (long long)(&mystat)->st_size, datestring);

}

void list_files(char *the_path,int is_all,int Rec) {
	DIR *mydir;

	mydir = opendir(the_path);
	if (mydir == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	struct dirent *files;
	for (;;) {
		files = readdir(mydir);
		if (files == NULL) {
			break;
		}
		// -a 选项
		if (is_all == 0){
			if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0 || files->d_name[0] == '.') {
				continue;
			}
		}
		char s[strlen(the_path) + strlen(files->d_name) + 1];
		strcpy(s, the_path);
		strcat(s,files->d_name);
		get_stat(s,is_all,Rec);
		printf("\t%s", files->d_name);
		printf("\n");
	}
	if (closedir(mydir) == -1) {
		perror("closedir");
	}
//	exit(EXIT_SUCCESS);
}


int test_search(char * options){
	char * target[4] = {"-R","-a","-aR","-Ra"};
	int res;
	int num = -1;
	for (int i = 0;i < 4;i++){
		res = strcmp(options,target[i]);
		if (res == 0){
			num = i;
			break;
		}
	}
	return num;
}

