# apue
apue-example

##ch8

进程控制
- 创建新进程
- 执行程序
- 进程终止

###进程标识

进程的id是唯一的，可以延迟复用

###fork
```
#include <unistd.h>
pid_t fork(void);
```
fork创建子进程，fork调用一次，返回两次，子进程返回0，父进程返回子进程id。
子进程获得父进程数据空间、堆和栈的**副本**，但是父进程和子进程并不共享这些存储空间部分，父进程和子进程共享正文段。
