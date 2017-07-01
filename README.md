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


### exit
##### 进程有下面5种正常终止方式：

1. 在main函数内执行return语句。这等效于调用exit。

2. 调用exit函数。此函数有ISO C定义，其操作包括调用各终止处理程序（终止处理程序在调用atexit函数时登记），然后关闭所有标准I/O流等。

3.调用_exit或_Exit函数。ISO C定义_Exit，其目的是为进程提供一种无需运行终止处理程序或信号处理程序而终止的方法。对标准I/O流是否进行冲洗，这取决于实现。在UNIX系统中。函数_Exit和_exit是同义的，并不清洗标准I/O流。函数_exit由exit调用，它处理UNIX特定的细节。在大多数UNIX系统实现中，exit（3）是标准C库中的一个函数，而_exit（2）则是一个系统调用。

4. 进程的最后一个线程在其启动例程中执行返回语句。但是，该线程的返回值不会用作进程的返回值。当最后一个线程从其启动例程返回时，该进程以终止状态0返回。

5. 进程的最后一个线程调用pthread_exit函数。在这种情况下，进程终止状态总是0，这与传送给pthread_exit的参数无关。

##### 三种异常终止方式如下：

1. 调用abort。它产生SIGABRT信号，这是下一种异常终止的特例。

2. 当进程接收到某些信号时。信号可由进程自身（例如调用abort函数）、其他进程或内核产生。

3. 最后一个线程对“取消”（cancellation）请求作出响应。按系统默认，“取消”以延迟方式发生：一个线程要求取消另一个线程，一段时间之后，目标线程终止。

**不管进程如何终止，最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等。**

该终止进程的父进程都能用wait或waitpid函数取得其终止状态。

子进程是在父进程fork之后生成的，子进程终止后将其终止状态返回给父进程，若父进程在子进程之前终止，则子进程的父进程都改变为init进程，保证了每个进程都有父进程

子进程在父进程前终止，那么父进程如何能在做响应检查时得到子进程的终止状态呢？内核为每个终止的进程保存一定的信息，当终止进程的父进程调用wait或者waitpid时，可以得到这些信息。

僵死进程：进程已经终止，但是其父进程尚未对其进行善后处理（获取终止进程的有关信息、释放它仍占用的资源），称之为僵死进程（zombie）

init被编写为无论何时其子进程终止，init就调用wait来取得其终止状态，避免了僵死进程，值得注意的是，init的子进程即可能指init自己产生的进程，也可能是其收养的进程。

### wait和waitpid
当进程终止后，会向父进程发送SIGCHLD信号，父进程可以选择忽略或者提供一个执行函数。系统默认动作是忽略。

当调用wait或者waitpid时，会发生三种情况：
1. 如果其所有子进程都在运行，阻塞
2. 如果一个子进程终止，取得该进程的终止状态并立即返回
3. 没有子进程，立即出错返回。

```
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
```
**检查wait和waitpid所返回的终止状态的宏**

|宏|说明|
|--|--|
|WIFEXITED(status)|若为正常终止子进程返回的状态，则为真。对于这种情况可执行WEXITSTATUS(status)，取子进程传送给exit、_exit或_Exit参数的低8位|
|WIFSIGNALED(status)|若为异常终止子进程返回的状态，则为真（接到一个不捕捉的信号）。对于这种情况，可执行WTERMSIG(status)，取使子进程终止的信号编号。另外，有些实现定义宏WCOREDUMP(status)，若已产生终止进程的core文件，则它返回真|
|WIFSTOPPED(status)|若为当前暂停子进程的返回状态，则为真。对于这种情况，可执行WSTOPSIG(status)，取使子进程暂停的信号编号|
|WIFCONTINUED(status)|若在作业控制暂停后已经继续的子进程返回了状态，则为真。（POSIX.1的XSI扩展；仅用于waitpid。）|

**wait和waitpid的区别**
1. 在一个子进程终止之前，wait使其调用者阻塞，waitpid有一选项，可使调用者不阻塞。
2. waitpid并不等待其调用之后的第一个终止的子进程，有若干选型，可以控制等待的子进程。

**waitpid options常量**

|常量|说明|
|--|--|
|WCONTINUED|若实现支持作业控制，那么由pid指定的任一子进程在暂停后已经继续，但其状态尚未报告，则返回其状态|
|WNOHANG|若由pid指定的子进程并不是立即可用的，则waitpid不阻塞，此时其返回值为0|
|WUNTRACED|若某实现支持作业控制，而由pid指定的任一子进程已处于暂停状态，并且其状态自暂停以来还未报告过，则返回其状态。WIFSTOPPED宏确定返回值是否对应于一个暂停子进程|

waitpid提供了wait函数没有的三个功能
1. 等待一个特定的进程
2. 提供了一个wait的非阻塞版本，有时希望获取一个子进程的状态，但是不想阻塞
3. waitpid通过WUNTRACED和WCONTINUED选项支持作业控制


### waitid
waitid也可以取得进程终止状态，比waitpid更加灵活。
```
#include <sys/wait.h>
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```
**waitid的idtype常量**

|常量|说明|
|--|--|
|P_PID|等待一个特定的进程：id包含要等待子进程的进程ID|
|P_PGID|等待一个特定进程组中的任一个子进程：id包含要等待子进程的进程组ID|
|P_ALL|等待任一个子进程：忽略id|

**waitid的options常量**

|常量|说明|
|--|--|
|WCONTINUED|等待一个进程，它以前曾被暂停，此后又已继续，但其状态尚未报告|
|WEXITED|等待已退出的进程|
|WNOHANG|如无可用的子进程退出状态，立即返回而非阻塞|
|WNOWAIT|不破坏子进程退出状态。该子进程退出状态可由后续的wait、waitid或waitpid调用取得|
|WSTOPPED|等待一个进程，它已经暂停，但其状态尚未报告|

infop参数指向siginfo结构的指针，该结构包含了造成子进程状态改变有关信号的详细信息。

### wait3和wait4
wait3和wait4提供的功能比wait、waitpid和waitid所提供的功能要多一个，这与附加参数rusage有关。该参数要求内核返回由终止进程及其所有子进程使用的资源汇总。
```
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
pid_t wait3(int *statloc, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *statloc, int options, struct rusage *rusage);
//两个函数返回值:若成功返回进程ID，若出错则返回-1
```
资源统计信息包括用户CPU时间总量、系统CPU时间总量、页面出错次数、接收到信号的次数等

### 竞争状态

多个进程都企图对共享数据进行某种处理，而最后的结果又取决于进程运行的顺序时，则我们认为这发生了竞争条件（race condition）。如果在fork之后的某种逻辑显式或隐式地依赖于在fork之后是父进程先运行还是子进程先运行，那么fork函数就会是竞争条件活跃的滋生地。通常，我们不能预料哪一个进程先运行。即使知道哪一个进程先运行，那么在该进程开始运行后，所发生的事情也依赖于系统负载以及内核的调度算法。

### exec

有7个exec函数可供调用
```
#include <unistd.h>
int execl(const char *path, const char *arg, ...)
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execlp(const char *file, const char *arg, ...);
int execvp(const char *file, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp);
```

###解释器文件

起始行形式：
```#! pathname [optional-argument]```

常见的是：
```#! /bin/sh```

### system函数

```
#include<stdlib.h>
int system(const char* cmdstring);
```
**函数说明**

system（）会调用fork（）产生子进程，由子进程来调用/bin/sh -c string来执行参数string字符串所代表的命令，此命令执行完后随即返回原调用的进程。在调用system（）期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被忽略。

**返回值**

- 如果fork（）失败 返回-1：出现错误

- 如果exec（）失败，表示不能执行Shell，返回值相当于Shell执行了exit（127）

- 如果执行成功则返回子Shell的终止状态

如果system（）在调用/bin/sh时失败则返回127，其他失败原因返回-1。若参数string为空指针（NULL），仅当命令处理程序可用时，返回非零值，可以通过这一特征判断在一个给定的操作系统上是否支持system函数(当system函数返回值为0时，表明system函数无效，在UNIX系统中，system函数总是可用的)；。如果system（）调用成功则最后会返回执行shell命令后的返回值，但是此返回值也有可能为 system（）调用/bin/sh失败所返回的127，因此最好能再检查errno 来确认执行成功。
