#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/my_err.h"

int globvar = 6;
char buf[] = "a write to stdout\n";
int main()
{
	int var;
	pid_t pid;

	var = 88;
	if(write(STDOUT_FILENO,buf,sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");

	if((pid = fork()) < 0){
		err_sys("fork error");
	}else if (pid == 0){//child
		printf("child proc id is %ld\n",(long)getpid());
		globvar++;
		var++;
	}else{
		printf("parent proc id is %ld\n",(long)getpid());
		sleep(2);
	}

	printf("pid = %ld, glob = %d, var = %d\n",(long)getpid(), globvar, var);
	exit(0);
}
