#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// --
#include "child.h"

// 创建页面




int main() {
  printf("---------------------------\n");
  printf("MYSH\n");
  pid_t pid;
  int this_status;
  for (;;) {
	pid = fork();
	if (pid < 0) {
	  perror("fork");
	  exit(EXIT_FAILURE);
	}
	if (pid == 0) {
	  // child
	  read_command();
	} else {
	  // father
	  waitpid(pid, &this_status, WUNTRACED);
	  printf("<%d> = [%d]\n",pid,WIFEXITED(this_status));
	}
  }
}
