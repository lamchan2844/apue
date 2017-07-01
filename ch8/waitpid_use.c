#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/my_err.h"
#include "pr_exit.h"
int main()
{
	pid_t pid;
	if((pid = fork())<0){
		err_sys("fork error");
	}else if(pid == 0){
		if((pid = fork())<0){
			err_sys("fork error");
		}else if(pid >0){
			exit(0);
		}
		sleep(2);
		printf("second child ,parent pid = %ld\n",(long)getppid());
		exit(0);
	}
	if(waitpid(pid,NULL,0)!=pid){
		err_sys("waitpid error");
	}

	exit(0);
}
