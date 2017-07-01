#include <stdlib.h>
#include <sys/wait.h>
#include "../include/my_system.h"
#include "../include/my_err.h"
#include "../include/pr_exit.h"

int main()
{
	int status;

	if((status = my_system("date")) < 0){
		err_sys("system() error");
	}
	pr_exit(status);
	
	if((status = my_system("nosuchcommand")) < 0){
		err_sys("system() error");
	}
	pr_exit(status);

	if((status = my_system("who;exit 44")) < 0){
		err_sys("system() error");
	}
	pr_exit(status);

	exit(0);

}
