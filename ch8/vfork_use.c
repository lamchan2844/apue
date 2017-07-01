#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/my_err.h"

int globvar = 6;
int main()
{
	int var;
	pid_t pid;

	var = 88;
	printf("before fork\n");

	if((pid = vfork()) < 0){
		err_sys("vfork error");
	}else if (pid == 0){//child
		printf("child proc id is %ld\n",(long)getpid());
		globvar++;
		var++;
		_exit(0);	//child terminates;
		/*_exit 并不执行标准I/O缓冲区的冲洗操作，若调用了exit，程序输出不确定，依赖于标准I/O的实现。可能的一个结果是：子进程在exit后关闭了标准输出文件描述符，父进程的printf返回-1，没有输出*/
	}
	printf("parent proc id is %ld\n",(long)getpid());

	printf("pid = %ld, glob = %d, var = %d\n",(long)getpid(), globvar, var);
	exit(0);
}
