#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/my_err.h"
char *env_init[] = {"USER=unknown","PATH=/tmp",NULL};
int main()
{
	pid_t pid;
	if((pid = fork()) < 0){
		err_sys("fork error");
	}else if(pid == 0){
		if(execle("/root/code/court/apue/ch8/echoAll","echoAll","myarg1","MY ARG2",(char *)0, env_init)<0){
			err_sys("execle error");
		}
	}
	if(waitpid(pid,NULL,0) < 0){
		err_sys("wait error");
	}
	if((pid = fork())<0){
		err_sys("fork error");
	}else if(pid == 0){
		if(execlp("./echoAll","echoAll","only 1 arg",(char *)0)<0){
			err_sys("execlp error");
		}
	}
	if(waitpid(pid,NULL,0) < 0){
		err_sys("wait error");
	}
	exit(0);
}
