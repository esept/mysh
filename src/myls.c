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


int command_myls(char *path[], int length) {
	int all = 0, Rec = 0, i;
	char *default_path = "./"; // 默认路径

	// 首先处理所有选项
	for (i = 1; i < length; i++) {
		int res = test_search(path[i]);
		if (res == 1) {
			Rec = 1;
		} else if (res == 2) {
			all = 1;
		} else if (res == 3) {
			all = 1;
			Rec = 1;
		}
	}
	if (length == 1 || (length == 2 && (all || Rec))) {
		list_files("./", all, Rec);
	} else {
		for (int i = 1; i < length; i++) {
			if (test_search(path[i]) == 0) {
				list_files(path[i], all, Rec);
			}
		}
	}
	return 0;
} // preprocess command myls

void get_stat(char *file_path,int is_all,int rec) {
	int rt_stat;
	struct stat mystat;
	rt_stat = lstat(file_path, &mystat);
	if (rt_stat < 0){
		printf("%s",file_path);
		perror("lstat");
//		exit(EXIT_FAILURE);
		return;
	}
	char filemod[11];
	mode_t mod = (&mystat)->st_mode;

	if (rec == 1 && S_ISDIR(mod)) {
		char newpath[strlen(file_path)+2];
		strcpy(newpath,file_path);
		strcat(newpath,"/");
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


	printf("%s",filemod);

	struct passwd *pw = getpwuid((&mystat) -> st_uid);
	struct group *gr = getgrgid((&mystat) -> st_gid);

	char datestring[256];
	struct tm *tm;
	tm = localtime(&(&mystat) -> st_mtime);
	strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);

	char *color = FILE_COLOR;
	if (S_ISDIR(mystat.st_mode)) {
		color = DIRECTORY_COLOR;
	} else if (mystat.st_mode & S_IXUSR) {
		color = EXECUTABLE_COLOR;
	}

	char *filename = strrchr(file_path, '/');
	if (filename == NULL) {
		filename = file_path;
	} else {
		filename++; // 跳过 '/'
	}

	printf(" %lu \t%s \t%s \t%lld \t%s", (unsigned long)(&mystat)->st_nlink, pw->pw_name, gr->gr_name, (long long)(&mystat)->st_size, datestring);
	printf("\t%s%s%s\n", color, filename, RESET_COLOR);

} // get + show all stat

void list_files(char *the_path, int is_all, int Rec) {
	DIR *mydir = opendir(the_path);
	if (mydir == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	struct dirent *files;
	while ((files = readdir(mydir)) != NULL) {
		if (!is_all && files->d_name[0] == '.') {
			continue; // dont show invisiable file
		}

		char s[CMDLEN];
		snprintf(s, sizeof(s), "%s/%s", the_path, files->d_name); // get the real path

		get_stat(s, is_all, Rec); // get states of file/folders
	}

	if (closedir(mydir) == -1) {
		perror("closedir");
	}
} // get all files / folders

int test_search(char *option) {
	if (strcmp(option, "-R") == 0) {
		return 1;
	} else if (strcmp(option, "-a") == 0) {
		return 2;
	} else if (strcmp(option, "-aR") == 0 || strcmp(option, "-Ra") == 0) {
		return 3;
	}
	return 0;
} // test use -a or -R