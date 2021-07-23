- int main(int argc,char* argv[])
  - argc是命令行总的参数个数
  - argv[]是argc个参数，其中第0个参数是程序的全名，以后的参数命令行后面跟的用户输入的参数



## 下载客户端项目

- https://www.jb51.net/article/78160.htm





## httpd⭐

- https://github.com/EZLippi/Tinyhttpd
- https://blog.csdn.net/jcjc918/article/details/42129311
- https://blog.csdn.net/baiwfg2/article/details/45582723⭐
- https://blog.csdn.net/u014694510/article/details/86550044

## 流程

- main -> **startup** //创建服务器套接字，listen

- accept 得到客户端socket 传至**accept_request**

- while(1){accept -> accept_request}    执行**get_line**，得到client, path, method, query_string

  - get_line 执行 n = recv(sock, &c, 1, 0);
  - cgi = 0  执行**serve_file** -> 执行**header**,返回200&server信息  -> 执行**cat** 读取文件  返回客户端

- cgi = 1  **execute_cgi**

  - <img src="http://img.blog.csdn.net/20141226161119981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center" alt="">

- 错误状态

  - bad_request

    - if (content_length == -1) 
    - 400 BAD REQUEST

  - cannot_execute

    - if (pipe(cgi_output) < 0) /  if (pipe(cgi_input) < 0)  /if ( (pid = fork()) < 0 ) 
          cannot_execute(client);
    - HTTP/1.0 500 Internal Server Error

  - error_die

    - httpd = socket(PF_INET, SOCK_STREAM, 0); //创建套接字
    - httpd/setsockopt/bind/getsockname/listen/accept <0/=-1时执行
    - perror(sc)并exit(-1)
    - 

  - not_found

    - if (stat(path, &st) == -1) {

      ​    while ((numchars > 0) && strcmp("\n", buf)) /* read & discard headers */

      ​      numchars = get_line(client, buf, sizeof(buf));

      ​    not_found(client); //404错误

        }

    - stat(path, &st) == -1)

      - 获取文件信息失败 404

  - unimplemented

    - if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))

      {

      ​    unimplemented(client); //501返回未执行错误

      ​    return;

        }

    - 不是GET/POST方法  501 服务器无法执行

## 网络编程

- ss

  - 需要一个socket作为文件
  - 需要sockaddr_in->sockaddr  获得IP地址和端口号
  - bind将地址信息绑定到socket上
    - connect与bind相同
  - listen 让socket处于监听状态，并设置消息队列的最大长度
  - accept与bind相同，不过需要定义一个新的sockaddr_in。
    - new_temp_socket = accept(socket,(强转) new_sockaddr_in, sizeof(.))
    - 用一个新套接字接收
  - write
    - write(new_temp_socket, 信息，sizeof(信息))
  - close
    - close(new_temp_socket)
    - close(socket)

- socket编程

  - ```c++
    #include <stdio.h>
    #include <string.h>  //memset
    #include <stdlib.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>  //sockaddr_in
    int main(){
        //创建套接字
        //参数 AF_INET 表示使用 IPv4 地址，SOCK_STREAM 表示使用面向连接的套接字
        //IPPROTO_TCP 表示使用 TCP 协议
        //在 Linux 中，可以使用 write() / read() 函数进行 I/O 操作
        int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        //将套接字和IP、端口绑定
        // bind() 函数将套接字 serv_sock 与特定的 IP 地址和端口绑定
        //IP 地址和端口都保存在 sockaddr_in 结构体中
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
        serv_addr.sin_family = AF_INET;  //使用IPv4地址 <sys/socket.h>
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
        serv_addr.sin_port = htons(1234);  //端口 <netinet/in.h>
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        //进入监听状态，等待用户发起请求
        listen(serv_sock, 20);
        //接收客户端请求
        struct sockaddr_in clnt_addr;
        // socklen_t <sys/socket.h>  #include <unistd.h>
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        //向客户端发送数据
        char str[] = "http://c.biancheng.net/socket/";
        write(clnt_sock, str, sizeof(str));
       
        //关闭套接字
        close(clnt_sock);
        close(serv_sock);
        return 0;
    }
    ```

  - ```c++
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    int main(){
        //创建套接字
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        //向服务器（特定的IP和端口）发起请求
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
        serv_addr.sin_family = AF_INET;  //使用IPv4地址
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
        serv_addr.sin_port = htons(1234);  //端口
        connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
       
        //读取服务器传回的数据
        char buffer[40];
        read(sock, buffer, sizeof(buffer)-1);
       
        printf("Message form server: %s\n", buffer);
       
        //关闭套接字
        close(sock);
        return 0;
    }
    ```

- socket

  - int serv_sock = **socket**(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    - af  - AF_INET   AF_INET6 
      -  AF_INET 表示 IPv4 地址，例如 127.0.0.1；
      -  AF_INET6 表示 IPv6 地址，例如 1030::C9B4:FF12:48AA:1A2B。
      -  PF和AF一样
    - type - SOCK_STREAM   SOCK_DGRAM
      - 流格式套接字/面向连接的套接字
      - 数据报套接字/无连接的套接字
    - protocol - IPPROTO_TCP IPPTOTO_UDP  0 
      - 表示传输协议，常用的有 IPPROTO_TCP 和 IPPTOTO_UDP 
      - protocol由type决定  0可以自动推算
  - bind()  listen()



- sockaddr_in 

  - ```c
    struct sockaddr_in{
        sa_family_t     sin_family;   //地址族（Address Family），也就是地址类型
        uint16_t        sin_port;     //16位的端口号
        struct in_addr  sin_addr;     //32位IP地址
        char            sin_zero[8];  //不使用，一般用0填充
    };
    
    struct in_addr{
        in_addr_t  s_addr;  //32位的IP地址
    };
    ```

    - serv_addr.sin_family = AF_INET;  //使用IPv4地址

    - serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址

      - ```c
        unsigned long ip = inet_addr("127.0.0.1");
        printf("%ld\n", ip); //16777343
        ```

    - serv_addr.sin_port = htons(1234);  //端口



- memset
  -  作用是在一段内存块中填充某个给定的值，它对较大的结构体或数组进行清零操作的一种最快方法。 
  -  memset(&serv_addr, 0, **sizeof**(serv_addr)); 



- bind
  - 将地址相关绑定到套接字上
  - bind(serv_sock, (**struct** sockaddr*)&serv_addr, **sizeof**(serv_addr)); 
    - sock 为 socket 文件描述符
    - addr 为 sockaddr 结构体变量的指针
    - addrlen 为 addr 变量的大小，可由 sizeof() 计算得出
  - sockaddr强转sockaddr_in
    -  ![img](http://c.biancheng.net/uploads/allimg/190219/112PA615-1.jpg) 
    -  sockaddr 是一种通用的结构体，可以用来保存多种类型的IP地址和端口号
    -  而 sockaddr_in 是专门用来保存 IPv4 地址的结构体 
    -  另外还有 sockaddr_in6，用来保存 IPv6 地址 



- listen

  - listen(serv_sock, 20); 

  - int listen(int sock, int backlog);  //Linux

  - sock 为需要进入监听状态的套接字，backlog 为请求队列的最大长度。 

  - 请求队列

    - ```
      请求队列
      当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列（Request Queue）。
      
      缓冲区的长度（能存放多少个客户端请求）可以通过 listen() 函数的 backlog 参数指定，但究竟为多少并没有什么标准，可以根据你的需求来定，并发量小的话可以是10或者20。
      
      如果将 backlog 的值设置为 SOMAXCONN，就由系统来决定请求队列长度，这个值一般比较大，可能是几百，或者更多。
      
      当请求队列满时，就不再接收新的请求，对于 Linux，客户端会收到 ECONNREFUSED 错误，对于 Windows，客户端会收到 WSAECONNREFUSED 错误。
      ```

      



- connect

  -  connect(sock, (**struct** sockaddr*)&serv_addr, **sizeof**(serv_addr)); 
  -  同bind

  

- accept

  -  int clnt_sock = accept(serv_sock, (**struct** sockaddr*)&clnt_addr, &clnt_addr_size); 



- write
  -  write(clnt_sock, str, **sizeof**(str)); 
  -  ssize_t write(int fd, const void *buf, size_t nbytes);
     -  fd 为要写入的文件的描述符，buf 为要写入的数据的缓冲区地址，nbytes 为要写入的数据的字节数。 
- read
  - read(sock, buffer, **sizeof**(buffer)-1);
  - ssize_t read(int fd, void *buf, size_t nbytes);



- close
  -  close(clnt_sock); 
  -  close(serv_sock); 







#### 回声客户端

#### 连续监听

- while(1)



#### 缓冲区

-  每个 socket 被创建后，都会分配两个缓冲区，输入缓冲区和输出缓冲区。 
   -  ![TCP套接字的I/O缓冲区示意图](http://c.biancheng.net/uploads/allimg/190219/1149355056-0.jpg) 
   -  这些I/O缓冲区特性可整理如下：
      - I/O缓冲区在每个TCP套接字中单独存在；
      - I/O缓冲区在创建套接字时自动生成；
      - 即使关闭套接字也会继续传送输出缓冲区中遗留的数据；
      - 关闭套接字将丢失输入缓冲区中的数据。

#### 阻塞模式

- write

  1) 首先会检查缓冲区，如果缓冲区的可用空间长度小于要发送的数据，那么 write()/send() 会被阻塞（暂停执行），直到缓冲区中的数据被发送到目标机器，腾出足够的空间，才唤醒 write()/send() 函数继续写入数据。

  2) 如果TCP协议正在向网络发送数据，那么输出缓冲区会被锁定，不允许写入，write()/send() 也会被阻塞，直到数据发送完毕缓冲区解锁，write()/send() 才会被唤醒。

  3) 如果要写入的数据大于缓冲区的最大长度，那么将分批写入。

  4) 直到所有数据被写入缓冲区 write()/send() 才能返回。

- 当使用 read()/recv() 读取数据时：
  1) 首先会检查缓冲区，如果缓冲区中有数据，那么就读取，否则函数会被阻塞，直到网络上有数据到来。

  2) 如果要读取的数据长度小于缓冲区中的数据长度，那么就不能一次性将缓冲区中的所有数据读出，剩余数据将不断积压，直到有 read()/recv() 函数再次读取。

  3) 直到读取到数据后 read()/recv() 函数才会返回，否则就一直被阻塞。

  这就是TCP套接字的阻塞模式。所谓阻塞，就是上一步动作没有完成，下一步动作将暂停，直到上一步动作完成后才能继续，以保持同步性。 

#### 粘包

- 假设我们希望客户端每次发送一位学生的学号，让服务器端返回该学生的姓名、住址、成绩等信息，这时候可能就会出现问题，服务器端不能区分学生的学号。例如第一次发送 1，第二次发送 3，服务器可能当成 13 来处理，返回的信息显然是错误的。

  这就是数据的“粘包”问题，客户端发送的多个数据包被当做一个数据包接收。也称数据的无边界性，read()/recv() 函数不知道数据包的开始或结束标志（实际上也没有任何开始或结束标志），只把它们当做连续的数据流来处理。 

- TCP基于流的传输，基于流的传输不认为消息是一条一条的，是无保护消息边界的（保护消息边界：指传输协议把数据当做一条独立的消息在网上传输，接收端一次只能接受一条独立的消息）。
  所谓无边界是指数据发送端发送的字节数，在数据接收端接受时并不一定等于发送的字节数，可能会出现粘包情况

  UDP则是面向消息传输的，是有保护消息边界的，接收方一次只接受一条独立的信息，所以不存在粘包问题。
  他不会对数据包进行合并发送（也就没有Nagle算法之说了），他直接是一端发送什么数据，直接就发出去了，既然他不会对数据合并，每一个数据包都是完整的（数据+UDP头+IP头等等发一次数据封装一次），如果接收方一次没有接受完数据，但是第二次的数据又发送过来，那么就会发生数据丢失问题，原先的数据将会被丢弃。‘’


#### shutdown

-  int shutdown(int sock, int howto); 
   - howto 在 Linux 下有以下取值：
     - SHUT_RD：断开输入流。套接字无法接收数据（即使输入缓冲区收到数据也被抹去），无法调用输入相关函数。
     - SHUT_WR：断开输出流。套接字无法发送数据，但如果输出缓冲区中还有未传输的数据，则将传递到目标主机。
     - SHUT_RDWR：同时断开 I/O 流。相当于分两次调用 shutdown()，其中一次以 SHUT_RD 为参数，另一次以 SHUT_WR 为参数。
-  shutdown() 用来关闭连接，而不是套接字，不管调用多少次 shutdown()，套接字依然存在，直到调用 close() / closesocket() 将套接字从内存清除。 
-  默认情况下，close()/closesocket() 会立即向网络中发送FIN包，不管输出缓冲区中是否还有数据，而shutdown() 会等输出缓冲区中的数据传输完毕再发送FIN包。也就意味着，调用 close()/closesocket() 将丢失输出缓冲区中的数据，而调用 shutdown() 不会。 



#### 文件传输

- ```c++
  char *filename = "D:\\send.avi";  //文件名
  FILE *fp = fopen(filename, "rb");  //以二进制方式打开文件
  
  while( (nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0 ){
      send(clntSock, buffer, nCount, 0);
  }
  
  //接收
  FILE *fp = fopen(filename, "wb"); 
  
  while( (nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0 ){
      fwrite(buffer, nCount, 1, fp);
  }
  fclose(fp);
  ```

  



#### 域名

- http://c.biancheng.net/view/vip_5870.html

- **struct** hostent *host = gethostbyname("www.baidu.com"); 

- ```c
  struct hostent{
      char *h_name;  //official name
      char **h_aliases;  //alias list
      int  h_addrtype;  //host address type
      int  h_length;  //address lenght
      char **h_addr_list;  //address list
  }
  
  //IP地址
  for(int i=0; host->h_addr_list[i]; i++){
      printf("IP addr %d: %s\n", i+1, inet_ntoa( *(struct in_addr*)host->h_addr_list[i] ) );
  }
  ```

  

#### 大小端

- htons()：host to network short，将 short 类型数据从主机字节序转换为网络字节序。
- ntohs()：network to host short，将 short 类型数据从网络字节序转换为主机字节序。
- htonl()：host to network long，将 long 类型数据从主机字节序转换为网络字节序。
- ntohl()：network to host long，将 long 类型数据从网络字节序转换为主机字节序。



#### UDP

- 和TCP区别

  - socket创建参数不同
  - 都会创建两个地址结构，一个用来发送，一个接收
  - UDP不会将sockaddr_in bind到socket上，所以send时候需要sockaddr_in参数
  - 同理， recvfrom 类似于accept

- ```c
  int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
   
  read(sock, buffer, sizeof(buffer)-1);
  recvfrom(sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
  
  write(clnt_sock, str, sizeof(str));
  sendto(sock, buffer, strLen, 0, &clntAddr, nSize);
  sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
  ```




## socket底层-内核协议栈

- https://blog.csdn.net/Rong_Toa/article/details/105349859



## 知识点

#### 库

- 库

  - ```c++
    #include <stdio.h>
    
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    
    #include <ctype.h>  //size_t time_t pid_t
    #include <strings.h>
    #include <string.h>
    
    #include <sys/stat.h>
    #include <pthread.h>
    #include <sys/wait.h>
    
    #include <stdlib.h>
    #include <stdint.h>
    
    ```

- <sys/socket.h> 头文件中 socket() 函数来创建套接字 

  - ```
    int socket(int af, int type, int protocol);
    
    ```

  - bind listen accept  close 

  - socklen_t   typedef int socklen_t; 

- \#include <sys/types.h>

  -  是Unix/Linux系统的基本系统数据类型的头文件，含有size_t，time_t，pid_t等类型。 

- \#include <netinet/in.h>

  - 定义数据结构**sockaddr_in**
  - in_addr_t 在头文件 <netinet/in.h> 中定义，等价于 unsigned long，长度为4个字节。
  - htons 
  - inet_addr
    - s_addr 是一个整数，而IP地址是一个字符串，所以需要 inet_addr() 函数进行转换 
    - 另外需要说明的是，sockaddr_in 中保存 IP 地址的成员为 32 位整数，而我们熟悉的是点分十进制表示法，例如 127.0.0.1，它是一个字符串，因此为了分配 IP 地址，需要将字符串转换为 4 字节整数。
    - inet_addr() 函数可以完成这种转换。inet_addr() 将字符串转换为 32 位整数 

- \#include <arpa/inet.h>

  -  提供IP地址转换函数 
  -  https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/arpa_inet.h.html

- \#include <unistd.h>

  - 提供通用的文件、目录、程序及进程操作的函数

  - read write sleep close fork。  

  - unistd.h在unix中类似于window中的windows.h! 

  - ```c
    ssize_t read(int, void *, size_t); // 读取文件使用
    int unlink(const char *);
    ssize_t write(int, const void *, size_t); // 写文件
    int usleep(useconds_t); // 进程休眠，单位为微妙
    unsigned sleep(unsigned); // 进程休眠，单位为秒
    
    int access(const char *, int); // 获取文件的权限
    unsigned alarm(unsigned);
    int chdir(const char *);
    int chown(const char *, uid_t, gid_t);
    int close(int); // 关闭文件
    size_t confstr(int, char *, size_t);
    void _exit(int);
    pid_t fork(void);
    
    ```

  - socklen_t 



- \#include <sys/stat.h>

  - https://www.jianshu.com/p/f718458f102e

  - 返回一个文件的所有

  - ```c
    struct stat { 
         dev_t st_dev; // 文件所在设备ID 
         ino_t st_ino; // 结点(inode)编号  
         mode_t st_mode; // 保护模式 
         nlink_t st_nlink; // 硬链接个数  
         uid_t st_uid; // 所有者用户ID  
         gid_t st_gid; // 所有者组ID  
         dev_t st_rdev; // 设备ID(如果是特殊文件) 
         off_t ** st_size;** // 总体尺寸，以字节为单位 
         blksize_t st_blksize; // 文件系统 I/O 块大小
         blkcnt_t st_blocks; // 已分配 512B 块个数
         time_t st_atime; // 上次访问时间 
         time_t st_mtime; // 上次更新时间 
         time_t st_ctime; // 上次状态更改时间 
    };
    
    ```

    

- \#include <pthread.h>

  - 提供多线程操作的函数

  - https://blog.csdn.net/u012503639/article/details/108200078

    - gcc program.o -o program -lpthread 

  - ```c++
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*) void *arg);	
    
    ```

    - 第一个参数为指向线程标识符的指针。
    - 第二个参数用来设置线程属性。
    - 第三个参数是线程运行函数的起始地址。
    - 最后一个参数是运行函数的参数。

  - ```c++
    #include <stdio.h>
    #include <stdlib.h>
    #include <pthread.h>
    #include <unistd.h>
     
    #define    NUM_THREADS     8
     
    void *PrintHello(void *args) //任意类型指针
    {
        int thread_arg;
        sleep(1);
        thread_arg = (int)(*((int*)args));
        printf("Hello from thread %d\n", thread_arg);
        return NULL;
    }
     
    int main(void)
    {
        int rc,t;
        pthread_t thread[NUM_THREADS]; //pthread_t类型的数组，初始化8个线程
     
        for( t = 0; t < NUM_THREADS; t++)
        {
            printf("Creating thread %d\n", t);
            //此处t变量的用法是方便大家调测代码的写法，实际使用会有问题，因为这个t是局部变量，
            函数执行完后马上释放，大家传递参数时需要使用全局变量或malloc出来的变量。
            rc = pthread_create(&thread[t], NULL, PrintHello, &t);
            if (rc) //成功，返回0；若出错，返回出错编号
            {
                printf("ERROR; return code is %d\n", rc);
                return EXIT_FAILURE;
            }
        }
        sleep(5);
        for( t = 0; t < NUM_THREADS; t++)
            pthread_join(thread[t], NULL);
        return EXIT_SUCCESS;
    }
    
    ```

  - 资源回收  join detach

    - pthread_join(thread[t], NULL); 调用线程阻塞，直到回收线程结束
    - pthread_detach()

  - 线程属性

    - int pthread_attr_init(pthread_attr_t *attr);
      int pthread_attr_destory(pthread_attr_t *attr);

    - 绑定属性

      - ```c++
        #include <stdio.h>
        #include <pthread.h>
        //……
        int main( int argc, char *argv[] )
        {
            pthread_attr_t attr;
            pthread_t th;
            //……
            pthread_attr_init( &attr );
            //PTHREAD_SCOPE_SYSTEM（绑定的）和PTHREAD_SCOPE_PROCESS（非绑定的）。
            pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
            pthread_create( &th, &attr, thread, NULL );
            //……
        }
        
        ```

        

    - 分离属性

      -  pthread_attr_setdetachstat(pthread_attr_t *attr, int detachstate); 
      -  PTHREAD_CREATE_DETACHED（分离的）和PTHREAD_CREATE_JOINABLE 
      -  不需要再 join/detach

    - 调度属性

      -  pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy); 
      -  SCHED_RR（轮询）、SCHED_FIFO（先进先出）和SCHED_OTHER（其它） 

    - 堆栈大小属性

      - 分配大小
      - int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize); 

    - 满占警戒区大小属性 

###### wait

- https://blog.csdn.net/kc58236582/article/details/52248328

- \#include <sys/wait.h>

  - wait   wait_pid

  - https://blog.csdn.net/ialpha/article/details/2515793

  - https://blog.csdn.net/csdn_kou/article/details/81091191

  - ```c++
    pid_t wait(int *status);
    pid_t waitpid(pid_t pid, int *status, int options);
    
    ```

  - 

  - 会暂时停止目前进程的执行，直到有信号来到或子进程结束。如果在调用wait()时子进程已经结束，则wait()会立即返回子进程结束状态值 

  - 

  - https://blog.csdn.net/yellow_hill/article/details/40652987

  - 进程间等待

  - \#include <sys/ipc.h> 通讯
    \#include <sys/shm.h>  共享内存
    \#include <signal.h>  信号

- waitpid(pid, &status, 0);

  - pid
    - pid>0时，只等待**进程ID等于pid的子进程**，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
    - pid=-1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。
    - pid=0时，等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。
    - pid<-1时，等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。
  - options
    -  WNOHANG和WUNTRACED 
    -  如果使用了WNOHANG(wait no hung)参数调用waitpid，即使没有子进程退出，它也会立即返回，不会像wait那样永远等下去。 

- pid_t wait(int *status) 

  -  进程一旦调用了wait，就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。
     参数status用来保存被收集进程退出时的一些状态，它是一个指向int类型的指针。但如果我们对这个子进程是如何死掉的毫不在意，只想把这个僵尸进程消灭掉，（事实上绝大多数情况下，我们都会这样想），我们就可以设定这个参数为NULL，就象下面这样： 





- \#include <ctype.h>
  - 字符检查与转换
  - https://www.runoob.com/cprogramming/c-standard-library-ctype-h.html
- \#include <stdlib.h>
  - https://www.runoob.com/cprogramming/c-standard-library-stdlib-h.html
  - size_t  malloc NULL
- \#include <stdlo.h>
  - 输入输出   size_t
- \#include <stdint.h>
  - typedef int intptr_t; 
  - typedef unsigned int		uintptr_t; 
  - 在64位的机器上，intptr_t和uintptr_t分别是long int、unsigned long int的别名；
  - 在32位的机器上，intptr_t和uintptr_t分别是int、unsigned int的别名。 
  - 定义这两个数据类型别名也是为了“void *”指针 
- strings.h string.h string
  - string 和 string.h不同
  - 如果我们使用了string.h 这个头文件，那么我们不需要在进行包含这个 strings.h 这个文件。除非有一种情况。如果 没有定义 __USE_MISC这个变量，这个变量将会在 strings.h 头文件中进行定义。因为 string.h 中没有进行对这个变量进行定义 

#### 函数

###### pthread⭐

###### stat

- int stat(const char *path, struct stat *buf) 

- https://blog.csdn.net/qq_40839779/article/details/82789217

- ```c++
  struct stat st;
  stat(path, &st);
  st.st_mode & S_IFMT == S_IFDIR; //可以判断st是否是一个目录
  st.st_mode & S_IXUSR == 1;      //可以判断是否可执行
  
  ```

- S_IFMT  0170000   文件类型的位遮罩

  - S_IFSOCK    0140000     socket
    S_IFLNK     0120000     符号链接(symbolic link)
    S_IFREG     0100000     一般文件
    S_IFBLK     0060000     区块装置(block device)
    S_IFDIR     0040000     目录
    S_IFCHR     0020000     字符装置(character device)

- 权限

  - USR  GRP  OTH(other)  R W X
  - USR  S_IRWXU     00700       文件所有者的遮罩值(即所有权限值) 
    - S_IRUSR     00400       文件所有者具可读取权限
    - S_IWUSR     00200       文件所有者具可写入权限
    - S_IXUSR(S_IEXEC) 00100   文件所有者具可执行权限 
  - S_IRWXG     00070       用户组的遮罩值(即所有权限值) 
    -  S_IXGRP 00010       用户组具可执行权限 
  - S_IRWXO
    -  S_IXOTH 00001       其他用户具可执行权限 

- https://blog.csdn.net/ShelbyNotSabie/article/details/71173070

###### sprintf

- stdio
- https://www.runoob.com/cprogramming/c-function-sprintf.html

###### cgi

- ss
  -  [公共网关接口](https://baike.baidu.com/item/公共网关接口/10911997)（Common Gateway Interface，CGI）是Web 服务器运行时外部程序的规范，按CGI 编写的程序可以扩展服务器功能。CGI 应用程序能与[浏览器](https://baike.baidu.com/item/浏览器/213911)进行交互，还可通过数据API与[数据库服务器](https://baike.baidu.com/item/数据库服务器/613818)等外部数据源进行[通信](https://baike.baidu.com/item/通信/300982) 

###### execute_cgi⭐

- ss

  - void execute_cgi(int client, const char *path,  const char *method, const char *query_string)
    - path 是 htdocs + url
    - url是方法后的数据，到空格停止
    - query_string指向？后的第一个字符
    - method是GET/POST

- ss

  - 建立两个管道，cgi_input 和 cgi_output, 并 fork 一个进程。

    - int cgi_output[2];

      int cgi_input[2];

      pid_t pid;

    - 

  - 在子进程中，把 STDOUT 重定向到 cgi_outputt 的写入端，把 STDIN 重定向到 cgi_input 的读取端，关闭 cgi_input 的写入端 和 cgi_output 的读取端，设置 request_method 的环境变量，GET 的话设置 query_string 的环境变量，POST 的话设置 content_length 的环境变量，这些环境变量都是为了给 cgi 脚本调用，接着用 execl 运行 cgi 程序。

  - 在父进程中，关闭 cgi_input 的读取端 和 cgi_output 的写入端，如果 POST 的话，把 POST 数据写入 cgi_input，已被重定向到 STDIN，读取 cgi_output 的管道输出到客户端，该管道输入是 STDOUT。接着关闭所有管道，等待子进程结束。这一部分比较乱，见下图说明：

  

###### exit

-  exit(0)表示程序正常退出，非0表示非正常退出 
-  exit n = return n
-  exit 1

###### perror

- stdio

###### setsockopt

- https://blog.csdn.net/zhonglinzhang/article/details/9183229

- int ***\*getsockopt\****(int sock, int level, int optname, void *optval, socklen_t *optlen); 
- int ***\*setsockopt\****(int sock, int level, int optname, const void *optval, socklen_t optlen); 
  -  sock：将要被设置或者获取选项的套接字
  -  level：选项所在的协议层。
     - 1)SOL_SOCKET:通用套接字选项.
       2)IPPROTO_IP:IP选项.
       3)IPPROTO_TCP:TCP选项.　 
  -  optname：需要访问的选项名。
  -  optval：对于getsockopt()，指向返回选项值的缓冲。对于setsockopt()，指向包含新选项值的缓冲。
  -  optlen：对于getsockopt()，作为入口参数时，选项值的最大长度。作为出口参数时，选项值的实际长度。对于setsockopt()，现选项的长度。 

###### getsockname

- 

- getsockname() [函数](http://baike.baidu.com/view/15061.htm) 用于获取一个套接口的名字。它用于一个已捆绑或已连接套接口s，本地地址将被返 

###### htons

- host 主机字节序转换为网络字节序。
- 网络是大端 主机是小端/大端

###### cat

- fgets
  -  **char \*fgets(char \*str, int n, FILE \*stream)** 从指定的流 stream 读取一行，并把它存储在 **str** 所指向的字符串内。当读取 **(n-1)** 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止 
- while (!feof(resource)) 
  -  feof判断文件结束，如果文件结束，则返回非0值，否则返回0 
  -  EOF =-1 end of file这个定义的意思是，文档的结尾都有一个隐藏字符”EOF”，当程序读取它的时候，就会知道文件已经到达结尾。通常使用**while循环加EOF判断**作为读取结束的标志。 
  -  

###### size_t

-  size_t 是通过 typedef 声明的 unsigned int 类型 
-  ssize_t 在 "size_t" 前面加了一个"s"，代表 signed 

###### atoi

-  ascii to integer

###### get_line

- numchars = get_line(client, buf, sizeof(buf));

  - 返回字符数，将client内容读取到buf中
  - 逐字符读取，用i计数
  - 每次读取一行

- n = recv(sock, &c, 1, 0);

- while ((numchars > 0) && strcmp("\n", buf)) /* read & discard headers */

  ​      numchars = get_line(client, buf, sizeof(buf));

  - 每次读取一行，最后buf指向\n，**然后停止**
  - 循环，当是换行符时再次运行get_line
  - numchars重新计数了吗
    - int i = 0; 重新计数了
  - \n怎么在get_line运行的
    - char c = '\0';
    - while ((i < size - 1) && (c != '\n'))
  - 每次读取会覆盖吗？⭐
    - 如果返回值 = 0，则表示 str1 等于 str2。
    - =0不运行

###### socket底层⭐

- socket/accept/bind/recv/send
  - 返回-1即为出错。调用了exit(-1)

- recv MSG_PEEK
  -  recv的原型是ssize_t recv(int s, void *buf, size_t len, int flags); 通常flags都设置为0，此时recv函数读取tcp buffer中的数据到buf中，并从tcp buffer中移除已读取的数据。把flags设置为MSG_PEEK，仅把tcp buffer中的数据读取到buf中，并不把已读取的数据从tcp buffer中移除，再次调用recv仍然可以读到刚才读到的数据。 

###### http报文格式

- ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200417120646947.png) 

- 响应头部

  - Server：服务器应用软件的名称和版本

    Content-Type：响应正文的类型

    Content-Length：响应正文的长度

    Content-Charset：响应正文所使用的编码

    Content-Encoding：响应正文使用的数据压缩格式

    Content-Language：响应正文使用的语言

###### 计网⭐

- https://segmentfault.com/a/1190000019554576

###### 不会的地方

- content-length

- ```c
  buf[15] = '\0'; //15?
  if (strcasecmp(buf, "Content-Length:") == 0) //"Content-Length:"
      content_length = atoi(&(buf[16]));
  
  ```

  - Content-Length: 7+1+6+1=15  + '\0'
  - &(buf[16])  &a 一个地址
  - atoi asc -> int

- 父子进程共用管道和标准输入输出？

  - 

###### pipe

- ```c++
  #include<unistd.h>
  int pipe(int fd[2]);
  
  ```

  -  pipe函数定义中的fd参数是一个大小为2的一个数组类型的指针。该函数成功时返回0，并将一对打开的文件描述符值填入fd参数指向的数组。失败时返回 -1并设置errno。 
  -  通过pipe函数创建的这两个文件描述符 fd[0] 和 fd[1] 分别构成管道的两端，往 fd[1] 写入的数据可以从 fd[0] 读出。并且 fd[1] 一端只能进行写操作，fd[0] 一端只能进行读操作，不能反过来使用。要实现双向数据传输，可以使用两个管道 
  -  存储了两个指针  f[1]写  f[0]读
  -  

###### fork

- ss

  - ```c++
    int main ()
    {
        pid_t fpid; //fpid表示fork函数返回的值
        int count = 0;
        fpid = fork();
        if (fpid < 0)
            printf("error in fork!");
        else if (fpid == 0)
        {
            printf("i am the child process, my process id is %d/n", getpid());
            printf("我是爹的儿子/n");//对某些人来说中文看着更直白。
            count++;
        }
        else
        {
            printf("i am the parent process, my process id is %d/n", getpid());
            printf("我是孩子他爹/n");
            count++;
        }
        printf("统计结果是: %d/n", count);
        return 0;
    }
    
    ```

###### dup2

- ```c++
  dup2(oldfd, newfd)等效于
  close(oldfd)；
  fcntl(oldfd, F_DUPFD, newfd)；
  
  ```

###### putenv

- ss
  - \#include<stdlib.h> 
  - putenv()用来改变或增加环境变量的内容. 参数string 的格式为name＝value, 如果该环境变量原先存在, 则变量内容会依参数string 改变, 否则此参数内容会成为新的环境变量.
  - 返回值：执行成功则返回0
- getenv
  -  getenv()用来取得参数name 环境变量的内容. 参数name 为环境变量的名称, 如果该变量存在则会返回指向该内容的指针. 环境变量的格式为name＝value. 
  -  getenv("USER")  -> root

###### excel

- ss
  -   \#include <unistd.h> 
      -   int execl(const char * path, const char * arg, ...); 
  -   execl()用来执行参数path 字符串所代表的文件路径, 接下来的参数代表执行该文件时传递过去的argv(0), argv[1], ..., 最后一个参数必须用空指针(NULL)作结束. 
  -   返回值：如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中 

## ele⭐

- 见md文件