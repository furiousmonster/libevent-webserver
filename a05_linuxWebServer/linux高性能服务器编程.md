# linux高性能服务器编程

## linux安装配置

### yum

- yum list installed
  - 显示已经安装的软件包
- yum list tomcat
  - 查找可以安装的软件包
- yum install tomcat
  - 安装软件包
- yum remove tomcat
  - 卸载
- yum deplist tomcat
  - 列出软件包的依赖
- -y 自动应答yes
- yum info tomcat
  - info 显示软件包的描述信息和概要信息
- 升级软件包
  - yum update
  - yum update tomcat



### vim 

- Vim 的全局配置一般在`/etc/vim/vimrc`或者`/etc/vimrc`，对所有用户生效。用户个人的配置在`~/.vimrc`。

  如果只对单次编辑启用某个配置项，可以在命令模式下，先输入一个冒号，再输入配置

- ```bash
  syntax on
  set autoindent //按下回车键后，下一行的缩进会自动跟上一行的缩进保持一致
  set tabstop=2
  set number //行号
  ```

- 

#### 鼠标

- ```bash
  set mouse=a
  ```

#### 提示



### libevent

#### 安装

- https://blog.csdn.net/qq_31442743/article/details/111888598

- 详解 sample运行 https://blog.csdn.net/u012814856/article/details/104283474

- 1.在http://libevent.org/下载libevent-2.1.8-stable.tar.gz

  2.tar -zxvf libevent-2.1.8-stable.tar.gz

  3.cd libevent-2.1.8-stable

  4./configure --prefix=/usr --libdir=/usr/lib64

  5.make

  6.make install

- 检查

  - ss

- sample

  - ./hello-world

- 卸载

#### 编译连接

- gcc libevent-hello.c -levent
- 其他方式？

#### 运行错误

- ln -s /usr/local/lib/libevent-2.1.so.7 /usr/lib/llibevent-2.1.so.7
- ln -s /usr/local/lib/libevent-2.1.so.7 /usr/**lib64**/libevent-2.1.so.7



### cmake



### 客户端/服务端





## TCP/IP

### 协议族

- ![image-20210712145928644](D:\Aahlz\zStorage\typoraimgs\image-20210712145928644.png)

#### 数据链路 - ARP

- IP与MAC转换



#### 网络层-ICMP

- ![image-20210712150221526](D:\Aahlz\zStorage\typoraimgs\image-20210712150221526.png)

#### 网络层-IP

- ![image-20210712150139919](D:\Aahlz\zStorage\typoraimgs\image-20210712150139919.png)



### 报文封装



### DNS⭐

- ![image-20210712150715076](D:\Aahlz\zStorage\typoraimgs\image-20210712150715076.png)
- ![image-20210712154949029](D:\Aahlz\zStorage\typoraimgs\image-20210712154949029.png)

#### tcpdump

- ![image-20210712154859181](D:\Aahlz\zStorage\typoraimgs\image-20210712154859181.png)
- 

### IP

#### IP分片

- ![image-20210712221450522](D:\Aahlz\zStorage\typoraimgs\image-20210712221450522.png)



### TCP

#### 状态过程



#### time_wait

- 客户端
- ![image-20210712223042037](D:\Aahlz\zStorage\typoraimgs\image-20210712223042037.png)



#### 复位报文段



### 实例-访问服务器

#### 代理

- ![image-20210712223755055](D:\Aahlz\zStorage\typoraimgs\image-20210712223755055.png)



#### service

- ![image-20210712223935009](D:\Aahlz\zStorage\typoraimgs\image-20210712223935009.png)



#### squid





#### http请求

- ![image-20210712224339818](D:\Aahlz\zStorage\typoraimgs\image-20210712224339818.png)







- ![image-20210712224417797](D:\Aahlz\zStorage\typoraimgs\image-20210712224417797.png)



#### http应答

- ![image-20210712224526707](D:\Aahlz\zStorage\typoraimgs\image-20210712224526707.png)



#### http状态码

- ![image-20210712224546856](D:\Aahlz\zStorage\typoraimgs\image-20210712224546856.png)









## linux基础⭐

### 基础api

#### socket

- ![image-20210712224747043](D:\Aahlz\zStorage\typoraimgs\image-20210712224747043.png)

##### 大端字节序



##### 地址

- ![image-20210712225015702](D:\Aahlz\zStorage\typoraimgs\image-20210712225015702.png)
- ![image-20210712225028175](D:\Aahlz\zStorage\typoraimgs\image-20210712225028175.png)



###### 专用地址

- ![image-20210712225247814](D:\Aahlz\zStorage\typoraimgs\image-20210712225247814.png)
- ![image-20210712225322770](D:\Aahlz\zStorage\typoraimgs\image-20210712225322770.png)



###### 地址转换

- ![image-20210712225653009](D:\Aahlz\zStorage\typoraimgs\image-20210712225653009.png)





##### socket创建/bind/listen

- int
  - ![image-20210712225800943](D:\Aahlz\zStorage\typoraimgs\image-20210712225800943.png)
- bind
  - ![image-20210712225835644](D:\Aahlz\zStorage\typoraimgs\image-20210712225835644.png)
- listen
  - ![image-20210712225905801](D:\Aahlz\zStorage\typoraimgs\image-20210712225905801.png)



- 实际
  - ![image-20210712225954479](D:\Aahlz\zStorage\typoraimgs\image-20210712225954479.png)
  - ![image-20210712230149088](D:\Aahlz\zStorage\typoraimgs\image-20210712230149088.png)

###### close

- ![image-20210712230635096](D:\Aahlz\zStorage\typoraimgs\image-20210712230635096.png)
- ![image-20210712230646880](D:\Aahlz\zStorage\typoraimgs\image-20210712230646880.png)



### i/o函数

#### pipe

- ![image-20210712231150519](D:\Aahlz\zStorage\typoraimgs\image-20210712231150519.png)



#### dup



#### readv/writev

- 分散读/集中写
  - ![image-20210712231414104](D:\Aahlz\zStorage\typoraimgs\image-20210712231414104.png)



#### 零拷贝sendfile/splice/tee

- ![image-20210712231532788](D:\Aahlz\zStorage\typoraimgs\image-20210712231532788.png)

- splice
  - echo
    - ![image-20210712231840000](D:\Aahlz\zStorage\typoraimgs\image-20210712231840000.png)

- tee





#### mmap

- ![image-20210712231601316](D:\Aahlz\zStorage\typoraimgs\image-20210712231601316.png)



#### fcntl



### linux服务器程序规范

- ![image-20210712232058548](D:\Aahlz\zStorage\typoraimgs\image-20210712232058548.png)



#### 日志



#### 用户



#### 进程

- pid ppid(父) pgid(进程组) sid(会话)



#### 系统资源



#### 目录

- ![image-20210712232705323](D:\Aahlz\zStorage\typoraimgs\image-20210712232705323.png)



#### 守护进程

- ![image-20210712232828506](D:\Aahlz\zStorage\typoraimgs\image-20210712232828506.png)

- ```c
  bool daemonize() {
      // 创建子进程，关闭父进程，这样可以使程序后台运行
      pid_t pid = fork();
      if (pid < 0) {
          return false; //失败
      } else if (pid > 0) { //子进程
          exit(0);
      }
      
      /* 设置文件权限掩码，当进程创建新文件open(const char *pathname, int flags, mode_t mode),文件权限是mode & 0777 */
      
      /* 创建新会话，设置本进程为进程组首领*/
      
      pid_t sid = setsid();
      if (sid < 0) {
  		return false;
      }
      
      /* 切换工作目录*/
      if ((chdir("/")) < 0) {
          return false;
      }
      
      /* 关闭标准输入、输出、错误输出设备 */
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
      
      
      /* 关闭其他已经打开的文件描述符，代码省略 */
      
      /* 将标准输入、输出、错误定向到/dev/null文件 */
      open("/dev/null", O_RDONLY);
      open("/dev/null", O_RDWR);
      open("/dev/null", O_RDWR);
      return true;
  }
  ```

  

#### fork

- https://www.cnblogs.com/love-jelly-pig/p/8471206.html
-   **1）在父进程中，fork返回新创建子进程的进程ID；**
    **2）在子进程中，fork返回0；**
    **3）如果出现错误，fork返回一个负值；**



### 高性能服务器程序框架

#### C/S

- ![image-20210712234647779](D:\Aahlz\zStorage\typoraimgs\image-20210712234647779.png)
- ![image-20210712234659583](D:\Aahlz\zStorage\typoraimgs\image-20210712234659583.png)



#### 框架

- ![image-20210712234739205](D:\Aahlz\zStorage\typoraimgs\image-20210712234739205.png)

#### I/O模型

- 阻塞
  - 会stop and wait
  - ![image-20210712234949316](D:\Aahlz\zStorage\typoraimgs\image-20210712234949316.png)
- I/O复用
  - ![image-20210712235047158](D:\Aahlz\zStorage\typoraimgs\image-20210712235047158.png)



#### 事件处理reactor

- ![image-20210712235242929](D:\Aahlz\zStorage\typoraimgs\image-20210712235242929.png)
- reactor
  - ![image-20210712235320738](D:\Aahlz\zStorage\typoraimgs\image-20210712235320738.png)
  - ![image-20210712235346339](D:\Aahlz\zStorage\typoraimgs\image-20210712235346339.png)
- proactor



#### 并发模式

- ![image-20210712235701152](D:\Aahlz\zStorage\typoraimgs\image-20210712235701152.png)



### I/O复用

- ![image-20210713000311218](D:\Aahlz\zStorage\typoraimgs\image-20210713000311218.png)



#### select

- ![image-20210713104030148](D:\Aahlz\zStorage\typoraimgs\image-20210713104030148.png)

##### fd_set





#### poll

- ![image-20210713121453504](D:\Aahlz\zStorage\typoraimgs\image-20210713121453504.png)
- fds
  - ![image-20210713121517322](D:\Aahlz\zStorage\typoraimgs\image-20210713121517322.png)

- ![image-20210713121544892](D:\Aahlz\zStorage\typoraimgs\image-20210713121544892.png)

#### epoll

- epoll_create
  - 创建内核事件表![image-20210713121733194](D:\Aahlz\zStorage\typoraimgs\image-20210713121733194.png)
- epoll_ctl
  - 操作内核事件表
  - ![image-20210713121918784](D:\Aahlz\zStorage\typoraimgs\image-20210713121918784.png)
- epoll_wait
  - ![image-20210713122847126](D:\Aahlz\zStorage\typoraimgs\image-20210713122847126.png)





##### LT/ET

- **ET减少重复触发**

- LT
  - Level Trigger 电平触发
  - 默认，相当于高效poll
  - 对于采用LT工作模式的文件描述符，当epoll_wait检测到其上有事件发生并将此事件通知应用程序后，应用程序可以不立即处理该事件。这样，当应用程序下一次调用
    epoll_wait时，epoll_wait还会再次向应用程序通告此事件，直到该事件被处理。
- ET
  - Edge Trigger 边缘触发
  - 而对于采用ET工作模式的文件描述符，当epoll_wait检测到其上有事件发生并将此事件通知应用程序后，应用程序必须立即处理该事件，因为后续的epoll_wait调用将不再向应用程序通知这一事件。
  - 可见，**ET模式在很大程度上降低了同一个epoll事件被重复触发的次数，因此效率要比LT模式高**。



##### EPOLLONESHOT

- 即使我们使用ET模式，一个socket 上的某个事件还是可能被触发多次。这在并发程序中就会引起一个问题。比如-一-个线程（或进程，下同〉在读取完某个socket上的数据后开始处理这些数据，而在数据的处理过程中该socket上又有新数据可读（EPOLLIN再次被触发)，此时另外一个线程被唤醒来读取这些新的数据。于是就出现了两个线程同时操作一个socket的局面。这当然不是我们期望的。
- **我们期望的是一个socket连接在任一时刻都只被一个线程处理。这一点可以使用epoll的 EPOLLONESHOT事件实现**。
- 

#### 比较

- SS
  - **epoll使用一组函数**，而poll,select使用单个函数
  - epoll把用户关心的文件描述符上的事件放在**内核里的一个事件表中**，从而无须像select和 poll那样每次调用都要重复传人文件描述符集或事件集。但epoll需要使用一个额外的文件描述符，来唯一标识内核中的这个事件表。这个文件描述符使用如下epoll_create函数来创建
- ![image-20210713124407716](D:\Aahlz\zStorage\typoraimgs\image-20210713124407716.png)

##### poll/epoll

- ```c
  /* 如何索引poll返回的就绪文件描述符 */
  int ret = poll(fds, MAX_EVENT_NUMBER, -1);
  /* 必须遍历所有已注册文件描述符并找到其中就绪，可以用ret优化 */
  for (int i = 0; i < MAX_EVENT_NUMBER; ++i) {
      if (fds[i].revents & POLLIN) /*判断第i个文件描述符是否就绪*/
      {
          int sockfd = fds[i].fd;
          /* 处理sockfd */
      }
  }
  
  
  /* epoll */
  int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
  for (int i = 0; i < ret, ++i) {
      int sockfd = events[i].data.fd;
      /* 处理sockfd */
  }
  ```

  

#### 非阻塞connect

#### 聊天室

#### tcp/udp

#### xinetd



### 信号



### 定时器



### libevent

- ![image-20210713125020415](D:\Aahlz\zStorage\typoraimgs\image-20210713125020415.png)
- reactor框架
  - 句柄
    - IO框架库要处理的对象，即IO事件、信号和定时事件，统一称为事件源。一个事件源通常和-一-个句柄绑定在一起。句柄的作用是，当内核检测到就绪事件时，它将通过句柄来通知应用程序这一事件。在Linux环境下，IO事件对应的句柄是文件描述符，信号事件对应的句柄就是信号值。
  - 事件多路分发器
    - 事件的到来是随机的、异步的。我们无法预知程序何时收到一个客户连接请求，又亦或收到一个暂停信号。所以程序需要循环地等待并处理事件，这就是事件循环。在事件循环中，等待事件一般使用IO复用技术来实现。IO框架库一般将系统支持的各种I/O复用系统调用封装成统一的接口，称为事件多路分发器。事件多路分发器的demultiplex 方法是等待事件的核心函数，其内部调用的是select、poll、epoll_wait等函数。
    - 
  - 事件处理器
  - 具体事件处理器
  - reactor
    - ![image-20210713130208542](D:\Aahlz\zStorage\typoraimgs\image-20210713130208542.png)
    - ![image-20210713130218354](D:\Aahlz\zStorage\typoraimgs\image-20210713130218354.png)

- libevent源码
  - ![image-20210713130354276](D:\Aahlz\zStorage\typoraimgs\image-20210713130354276.png)
- 实例
  - 



#### 动态链接库

- https://blog.csdn.net/zgljl2012/article/details/79280877





### 多进程

#### fork

- ![image-20210713153202701](D:\Aahlz\zStorage\typoraimgs\image-20210713153202701.png)
- 子进程的代码与父进程完全相同，同时它还会复制父进程的效掂《堆双掂、伐双/T态数据)。数据的复制采用的是所谓的**写时复制**〈copy on wrlte，即八A江批佑么然心之进程或子进程）对数据执行了写操作时，复制才公发生（无定畎贝中队独口然分观'力h程分配内存并复制父进程的数据)。即便如此，如果我们在程序中分配了大量内存.判用fork时也应当十分谨慎，尽量避免没必要的内存分配和数据复制。
- 此外，创建子进程后，父进程中打开的文件描述符默认在子进程中也是打开的1，**描述符的引用计数加1**。不仅如此，父进程的用户根目录、当前工作目录等变量的引用计数
  均会加1。

#### exec



#### 僵尸进程处理/wait

- **在子进程结束运行之后，父进程读取其退出状态之前**，我们称该子进程处于僵尸态。
- 另外一种使子进程进人僵尸态的情况是:父进程结束或者异常终止，而子进程继续运行。此时子进程的 PPID将被操作系统设置为1，即init进程。init进程接管了该子进程，并等待它结束。在父进程退出之后，子进程退出之前，该子进程处于僵尸态。
- 

#### 管道





#### 信号量



#### 共享内存





#### 消息队列



#### Ipc命令



#### 进程间传递文件描述符







### 多线程

#### 概述

- 线程是程序中完成一个独立任务的完整执行序列，即一个可调度的实体。
- 根据运行环境和调度者的身份，线程可分为内核线程和用户线程。
  - 内核线程，在有的系统上也称为LWT(Light Weight Process，轻量级进程)，运行在内核空间，由内核来调度
  - 用户线程运行在用户空间，由线程库来调度。
  - 当进程的一个内核线程获得CPU的使用权时，它就加载并运行一个用户线程。可见**，内核线程相当于用户线程运行的“容器”**。
  - 一个进程可以拥有M个内核线程和N个用户线程，其中M≤N。并且在一个系统的所有进程中，M和N的比值都是固定的。按照M:N的取值
  - 线程的实现方式可分为三种模式:**完全在用户空间实现、完全出内核调度和双层调度**(two level scheduler)。



#### 创建与结束

- ![image-20210713204558744](D:\Aahlz\zStorage\typoraimgs\image-20210713204558744.png)
- pthread_exit
- pthread_join

### 进程池线程池





## 优化与监测

### 调试

#### 最大文件描述符

#### 调整内核参数

#### gdb

#### 压力测试

### 监测

#### tcpdump

#### lsof

#### nc

#### strace

#### netstat

#### vmstat

#### ifstat

#### mpstat

# 代码

### 5-4 listen

- ![image-20210723140408605](D:\Aahlz\zStorage\typoraimgs\image-20210723140408605.png)

### 5-5 accept

- ![image-20210723140504781](D:\Aahlz\zStorage\typoraimgs\image-20210723140504781.png)
- 