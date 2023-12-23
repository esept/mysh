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
			if (test_search(path[i]) == 0) { // 如果是路径
				list_files(path[i], all, Rec);
			}
		}
	}
	return 1;
}

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

	printf(" %lu \t%s \t%s \t%lld \t%s", (unsigned long)(&mystat)->st_nlink, pw->pw_name, gr->gr_name, (long long)(&mystat)->st_size, datestring);
	printf("\t%s%s%s\n", color, file_path, RESET_COLOR);

}

void list_files(char *the_path, int is_all, int Rec) {
	DIR *mydir = opendir(the_path);
	if (mydir == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	struct dirent *files;
	while ((files = readdir(mydir)) != NULL) {
		if (!is_all && files->d_name[0] == '.') {
			continue; // 跳过隐藏文件
		}

		char s[CMDLEN]; // 路径长度
		snprintf(s, sizeof(s), "%s/%s", the_path, files->d_name); // 正确构建路径

		get_stat(s, is_all, Rec); // 获取文件状态
	}

	if (closedir(mydir) == -1) {
		perror("closedir");
	}
}

int test_search(char *option) {
	if (strcmp(option, "-R") == 0) {
		return 1; // 仅-R
	} else if (strcmp(option, "-a") == 0) {
		return 2; // 仅-a
	} else if (strcmp(option, "-aR") == 0 || strcmp(option, "-Ra") == 0) {
		return 3; // -a 和 -R
	}
	return 0; // 无效选项或路径
}