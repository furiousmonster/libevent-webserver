## 00 环境

### linux

#### 进程关闭启动

- ps -ef | grep gunicorn进程时，一直有一个进程在运行
  - 那是grep进程自身，进程号会变好，kill时找不到
  - https://www.cnblogs.com/byx1024/p/12228735.html
- 关闭进程与启动进程
  - systemctl stop mysqld
  - systemctl start mysqld
  - systemctl stop nginx
  - service mysqld start



#### 系统使用

- top
- free



### git

- 添加权限

  - https://blog.csdn.net/u014183172/article/details/84136084

  - ssh-keygen -t rsa -C“youremail@example.com”

  - ssh -v git@github.com

  - ssh-agent -s

  - eval `ssh-agent -s`

  - ssh-add /root/.ssh/id_rsa

  - cat /root/.ssh /id_rsa.pub

    - 登陆GitHub，打开“Account settings”，“SSH Keys”页面：

      然后，点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容：

    - ssh git@github.com

  - ssh -T git@github.com

- 添加

  - ```
    git init
    git add README.md
    git commit -m "first commit"
    git branch -M main
    git remote add origin https://github.com/furiousmonster/linux-webserver-learn.git
    git push -u origin main
    ```

- 问题
  - error: src refspec main does not match any.
  - 

### 项目运行

- sh ./build.sh

  - cannot find -lmysqlclient

- gcc

  - ```
    gcc --version
    ```

  - 升级 https://www.cnblogs.com/dinghailong128/p/12153330.html

  - ```
    wget http://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-7.3.0.tar.xz
    ```

  - ```
    ./contrib/download_prerequisites //时间很久
    ```

- mysql

  - -lmysqlclient
    - -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
  - tail -n 20 /var/log/mysqld.log
  - ERROR 2002 (HY000): Can’t connect to local MySQL server through socket ‘/var/lib/mysql/mysql.sock’ (2)
    - find / -name mysql.sock
    - mysql -u root -p -S /Data/mydata/mysql.sock
    - chown -R mysql:mysql /Data/mydata/mysql.sock

- makefile

  - Makefile:8: *** missing separator. Stop.

    - Makefile的命令行必须以一个tab作为开头，而Makefile中变量的定义、赋值以及Makefile内定函数如$(error “strings”)都不能以TAB开头，不然make会将其作为命令来处理

    - cat -t Makefile

      - tab会被显示为`^I`

    - 原来这跟vim编辑器的配置有关系。我在vim的配置文件`~/.vimrc`使用了`set expandtab` 语句，该语句会自动把tab转换为空格，这就导致了上述错误。

      解决方案自然是注释掉这条语句`"set expandtab`，或者干脆删掉这条语句。最后要记得修改Makefile，删掉四个空格，重新键入tab，保存，退出。

  - make: *** No rule to make target `sysutil.o', needed by `MicroChatServer'. 

    - ss



### cmake

#### 安装

- yum install cmake
- example
  - https://github.com/ttroy50/cmake-examples/tree/master/01-basic/A-hello-cmake

#### 打包

- ```
  mkdir build
  
  cd build
  
  cmake ..
  
  make
  
  ./hello_cmake
  ```



#### cmakelist编辑

- 01 单一文件

  - 

  - cmakelist.txt

    - ```cmake
      cmake_minimum_required(VERSION 2.6)
      project (hello_cmake)
      add_executable(${PROJECT_NAME} main.cpp)
      ```

  - main.cpp

    - ```c++
      #include <iostream>
      
      int main(int argc, char *argv[])
      {
         std::cout << "Hello CMake!" << std::endl;
         return 0;
      }
      ```

    

- 02 含链接

  - ```
    B-hello-headers$ tree
    .
    ├── CMakeLists.txt
    ├── include
    │   └── Hello.h
    └── src
        ├── Hello.cpp
        └── main.cpp
    ```

  - ```cmake
    # Set the minimum version of CMake that can be used
    # To find the cmake version run
    # $ cmake --version
    cmake_minimum_required(VERSION 3.5)
    
    # Set the project name
    project (hello_headers)
    
    # Create a sources variable with a link to all cpp files to compile
    set(SOURCES
        src/Hello.cpp
        src/main.cpp
    )
    
    # Add an executable with the above sources
    add_executable(hello_headers ${SOURCES})
    
    # Set the directories that should be included in the build command for this target
    # when running g++ these will be included as -I/directory/path/
    target_include_directories(hello_headers
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/include
    )
    ```

    

- 静态库

  - ```
    $ tree
    .
    ├── CMakeLists.txt
    ├── include
    │   └── static
    │       └── Hello.h
    └── src
        ├── Hello.cpp
        └── main.cpp
    ```

  - ```cmake
    cmake_minimum_required(VERSION 3.5)
    project(hello_library)
    
    #Generate the static library from the library sources
    add_library(hello_library STATIC 
        src/Hello.cpp
    )
    
    target_include_directories(hello_library
        PUBLIC 
            ${PROJECT_SOURCE_DIR}/include
    )
    
    
    ############################################################
    # Create an executable
    ############################################################
    
    # Add an executable with the above sources
    add_executable(hello_binary 
        src/main.cpp
    )
    
    # link the new hello_library target with the hello_binary target
    target_link_libraries( hello_binary
        PRIVATE 
            hello_library
    )
    
    ```

    



- 共享库





### workbench

#### 安装

- wget http://blog.zyan.cc/soft/linux/webbench/webbench-1.5.tar.gz
  tar zxvf webbench-1.5.tar.gz
  cd webbench-1.5
  make && make install
- yum install ctags
- mkdir
  - mkdir命令是常用的命令，用来建立空目录，它还有2个常用参数：
    - -m, --mode=模式 设定权限<模式> (类似 chmod)，而不是 rwxrwxrwx 减 umask
    - -p, --parents 需要时创建上层目录，如目录早已存在则不当作错误



#### 使用

- webbench -c 500 -t 30 http://127.0.0.1/test.jpg

  　　参数说明：-c表示并发数，-t表示时间(秒)



### boost

- 教程 https://www.cnblogs.com/lidabo/p/9294874.html
- http://zh.highscore.de/cpp/boost/introduction.html

#### 安装

- yum install boost
- yum install boost-devel 
- yum install boost-doc



## 01 简单服务器实现

- https://blog.csdn.net/Hanani_Jia/article/details/83548547

- cpp

  - ```c
    
    //实现最简单的http服务端程序
    //返回hello world
    //http是应用协议，在传输层使用的是tcp协议所以我们的程序本质上是tcp服务器
    //我们http指定监听10000端口，所以在请求访问的时候也要手动指定否则默认80端口
     
    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<string.h>
    #include<errno.h>
    #include<sys/socket.h>
    #include<netinet/in.h>
     
    int main(int argc,char* argv[])
    {
        int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(sockfd<0)
        {
            perror("socket error\n");
            return -1;
        }
        struct sockaddr_in lst_addr;
        lst_addr.sin_family=AF_INET;
        lst_addr.sin_port=htons(9998);
        lst_addr.sin_addr.s_addr=inet_addr("192.168.76.130");
        socklen_t len=sizeof(struct sockaddr_in);
        int ret=bind(sockfd,(struct sockaddr*)&lst_addr,len);
        if(ret<0)
        {
            perror("bind error\n");
            return -1;
        }
        if(listen(sockfd,5)<0)
        {
            perror("listen error\n");
            return -1;
        }
        while(1)
        {
            struct sockaddr_in cli_addr;
            int newfd=accept(sockfd,(struct sockaddr*)&cli_addr,&len);
            if(newfd<0)
            {
                perror("accept error\n");
                continue;
            }
            char buff[1024]={0};
            int ret=recv(newfd,buff,1023,0);
            if(ret>0)
            {
                printf("req:%s\n",buff);
    			//这里我们打印的是浏览器HTTP的请求头部
            }
            //返回的http头部信息要包括
            //首行 HTTP/1.1 200 OK
            //头信息:
            // 空行
            // 正文
            char *rsp="<h1>hello world</h1>";
            memset(buff,0x00,1024);
            sprintf(buff,"%s\r\n%s%d\r\n%s\r\n\r\n%s%","HTTP/1.1 200 OK","Content-Length: ",strlen(rsp),"Content-Type:text/html:charset=UTF-8",rsp);
    		//这里我们返回的是简单的字符串hello world
    		//%s \r\n 第一行输出了HTTP/1.1 200 OK
    		//%S %d \r\n 这里第二行输出了Content-Length: strlen(rsp) 这里我们是想通过这个关键字告诉浏览器我们这次发送的数据是多少
    		//%s \r\n 这一行输出了Content-Type:text/html:charset=UTF-8 是告诉浏览器我们的文件格式和编码格式
    		//\r\n 这一行是HTTP头部头部和正文之间的空行
    		//%s 这里才是我们最终传输的数据
    		//这一部分的内容才真正算是我们的HTTP协议的部分，这里是我们用HTTP协议和浏览器交流的部分。
    		printf("%s", buff);
            send(newfd,buff,strlen(buff),0);
            close(newfd);
        }
    
    ```

    

- python

  - ```python
    import os
    import sys
    import socket
    import datetime
    
    class HTTPServer():
        def __init__(self, port=8080):
            print "HTTP server is initiating ..." + os.linesep
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server_socket.bind(("", port))
            
            
        def startServer(self):
            self.server_socket.listen(sys.maxint)
            print("HTTP server is started ..." + os.linesep)
            while True:
                client_socket, client_address = self.server_socket.accept()
                self.handle_client(client_socket, client_address)
        
        def handle_client(self, client_socket, client_address):
            
            request_data = client_socket.recv(2048)
            print datetime.datetime.strftime(datetime.datetime.now(),'%Y-%m-%d %H:%M:%S') + " : " + str(client_address) + " connected"
            print("HTTP request:" + os.linesep+ "----------------------------------------")
            print(request_data)
            
            httpResponseStr = self.generateResponseStr()
            
            print(os.linesep + "Sending response:" + os.linesep+ "----------------------------------------")
            print(httpResponseStr)
            client_socket.send(httpResponseStr)
            client_socket.close()
        
        def generateResponseStr(self):
            print "Please input the response string with HTTP format(:q to exit)"
            responseStr = ""
            while True:
                line = raw_input()
                if line.__eq__(":q"):
                    break
                responseStr = responseStr + line + os.linesep
            return responseStr
                  
    def main():
        http_server = HTTPServer(port=8080)
        http_server.startServer()
    
    main()
    
    ```






### nc



## 02 select

- select

  - ```c++
    int select(	int nfds, 
    			fd_set *readfds, 
    			fd_set *writefds,
                fd_set *exceptfds, 
                struct timeval *timeout);
     
        nfds:     	监控的文件描述符集里最大文件描述符加1，告诉内核检测前多少个文件描述符的状态
        readfds：   文件描述符集合，读数据到达，传入传出参数
        writefds：  文件描述符集合，写数据到达，传入传出参数
        exceptfds： 监控异常发生到达文件描述符集合,如带外数据到达异常，传入传出参数
        timeout：   定时阻塞监控时间，3种情况
                    1.NULL，永远等下去
                    2.设置timeval，等待固定时间
                    3.设置timeval里时间均为0，检查描述字后立即返回，轮询
        struct timeval {
            long tv_sec; /* seconds */
            long tv_usec; /* microseconds */
        };
    ```

    

- ss

  - ```c
    对监控的文件描述符集合操作：
        void FD_CLR(int fd, fd_set *set); //把文件描述符集合里fd清0
        int FD_ISSET(int fd, fd_set *set); //测试文件描述符集合里fd是否置1
        void FD_SET(int fd, fd_set *set); //把文件描述符集合里fd位置1
        void FD_ZERO(fd_set *set);         //把文件描述符集合里所有位清0
        
        
    成功：
    	返回就绪（可读、可写、异常）文件描述符的总数。
    超时：
    	返回0.
    失败：
    	返回-1，设置errno
    收到信号：
    	返回-1，errno = EINTR
    ```

    

  

- ![在这里插入图片描述](https://www.pianshen.com/images/544/bcae4e61b01efaf1a3049be00b6ac000.png)

- ```c++
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <arpa/inet.h>
  #include <ctype.h>
  
  #include "wrap.h"
  
  #define SERV_PORT 6666
  
  int main(int argc, char *argv[])
  {
      int i, j, n, maxi;
  
      int nready, client[FD_SETSIZE];                 /* 自定义数组client, 防止遍历1024个文件描述符  FD_SETSIZE默认为1024 */
      int maxfd, listenfd, connfd, sockfd;
      char buf[BUFSIZ], str[INET_ADDRSTRLEN];         /* #define INET_ADDRSTRLEN 16 */
  
      struct sockaddr_in clie_addr, serv_addr;
      socklen_t clie_addr_len;
      fd_set rset, allset;                            /* rset 读事件文件描述符集合 allset用来暂存 */
  
      listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  
      int opt = 1;
      setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
      bzero(&serv_addr, sizeof(serv_addr));
      serv_addr.sin_family= AF_INET;
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port= htons(SERV_PORT);
  
      Bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
      Listen(listenfd, 128);
  
      maxfd = listenfd;                                           /* 起初 listenfd 即为最大文件描述符 */
  
      maxi = -1;                                                  /* 将来用作client[]的下标, 初始值指向0个元素之前下标位置 */
      for (i = 0; i < FD_SETSIZE; i++)
          client[i] = -1;                                         /* 用-1初始化client[] */
  
      FD_ZERO(&allset);
      FD_SET(listenfd, &allset);                                  /* 构造select监控文件描述符集 */
  
      while (1) {   
          rset = allset;                                          /* 每次循环时都从新设置select监控信号集 */
          nready = select(maxfd+1, &rset, NULL, NULL, NULL);
          if (nready < 0)
              perr_exit("select error");
  
          if (FD_ISSET(listenfd, &rset)) {                        /* 说明有新的客户端链接请求 */
  
              clie_addr_len = sizeof(clie_addr);
              connfd = Accept(listenfd, (struct sockaddr *)&clie_addr, &clie_addr_len);       /* Accept 不会阻塞 */
              printf("received from %s at PORT %d\n",
                      inet_ntop(AF_INET, &clie_addr.sin_addr, str, sizeof(str)),
                      ntohs(clie_addr.sin_port));
  
              for (i = 0; i < FD_SETSIZE; i++)
                  if (client[i] < 0) {                            /* 找client[]中没有使用的位置 */
                      client[i] = connfd;                         /* 保存accept返回的文件描述符到client[]里 */
                      break;
                  }
  
              if (i == FD_SETSIZE) {                              /* 达到select能监控的文件个数上限 1024 */
                  fputs("too many clients\n", stderr);
                  exit(1);
              }
  
              FD_SET(connfd, &allset);                            /* 向监控文件描述符集合allset添加新的文件描述符connfd */
              if (connfd > maxfd)
                  maxfd = connfd;                                 /* select第一个参数需要 */
  
              if (i > maxi)
                  maxi = i;                                       /* 保证maxi存的总是client[]最后一个元素下标 */
  
              if (--nready == 0)
                  continue;
          } 
  
          for (i = 0; i <= maxi; i++) {                               /* 检测哪个clients 有数据就绪 */
  
              if ((sockfd = client[i]) < 0)
                  continue;
              if (FD_ISSET(sockfd, &rset)) {
  
                  if ((n = Read(sockfd, buf, sizeof(buf))) == 0) {    /* 当client关闭链接时,服务器端也关闭对应链接 */
                      Close(sockfd);
                      FD_CLR(sockfd, &allset);                        /* 解除select对此文件描述符的监控 */
                      client[i] = -1;
                  } else if (n > 0) {
                      for (j = 0; j < n; j++)
                          buf[j] = toupper(buf[j]);
                      Write(sockfd, buf, n);
                      Write(STDOUT_FILENO, buf, n);
                  }
                  if (--nready == 0)
                      break;                                          /* 跳出for, 但还在while中 */
              }
          }
      }
      Close(listenfd);
      return 0;
  }
  ```

  





## 02 poll





## 02 epoll





## 03 mysql

- ```c++
  #include <stdlib.h>
  #include <stdio.h>
  #include "mysql/mysql.h"
  
  int main( int argc, char *argv[] )
  {
      MYSQL *conn_ptr;
      conn_ptr = mysql_init( NULL );  /* 连接初始化 */
      if ( !conn_ptr )
      {
          fprintf( stderr, "mysql_init failed" );
          return(EXIT_FAILURE);
      }
  
      conn_ptr = mysql_real_connect( conn_ptr, "x.x.x.x", "root", "passwd", "dbname", 0, NULL, 0 );   /* 建立实际连接 */
      /* 参数分别为：初始化的连接句柄指针，主机名（或者IP），用户名，密码，数据库名，0，NULL，0）后面三个参数在默认安装mysql>的情况下不用改 */
      if ( conn_ptr ){
          printf( "Connection success" );
      }
      else{
          printf( "Connection failed" );
      }
  
      mysql_close( conn_ptr ); /* 关闭连接 */
  
      return(EXIT_SUCCESS);
  }
  ```

  

## 04 tinyhttpd