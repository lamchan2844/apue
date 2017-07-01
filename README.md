# apue
apue-example

## ch8

进程控制
- 创建新进程
- 执行程序
- 进程终止

### 进程标识

进程的id是唯一的，可以延迟复用

### fork
```
#include <unistd.h>
pid_t fork(void);
```
fork创建子进程，fork调用一次，返回两次，子进程返回0，父进程返回子进程id。
子进程获得父进程数据空间、堆和栈的**副本**，但是父进程和子进程并不共享这些存储空间部分，父进程和子进程共享正文段。

### vfork
1. 子进程在父进程的空间中运行，如果子进程修改数据、函数调用、或者没有掉用exec或者exit就返回可能带来未知结果。
2. vfork保证子进程先运行，在子进程调用exec或exit之后父进程才能被调度运行。
