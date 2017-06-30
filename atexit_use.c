/*************************************************************************
	> File Name: atexit_use.c
	> Author: Chen Lin
	> Mail: chenlin2844@hotmail.com
	> Created Time: 2017年06月30日 星期五 09时31分25秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
static void my_exit1(void);
static void my_exit2(void);

int main()
{
	if(atexit(my_exit2) != 0)
		err_sys("can't register my_exit2");
	if(atexit(my_exit1) != 0)
		err_sys("can't register my_exit1");
	printf("main is done\n");
	return 0;
}
static void my_exit1(){
	printf("first exit handler\n");
}
static void my_exit2(){
	printf("seconed exit handler\n");
}
