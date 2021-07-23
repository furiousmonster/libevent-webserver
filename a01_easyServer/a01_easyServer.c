//实现最简单的http服务端程序
//返回hello world
//http是应用协议，在传输层使用的是tcp协议所以我们的程序本质上是tcp服务器
//我们http指定监听10000端口，所以在请求访问的时候也要手动指定否则默认80端口
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
int main(int argc,const char *argv[])
{
    //参数 AF_INET 表示使用 IPv4 地址，SOCK_STREAM 表示使用面向连接的套接字
    //IPPROTO_TCP 表示使用 TCP 协议
    // int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket error\n");
        return -1;
    }
    
    // bind() 函数将套接字 serv_sock 与特定的 IP 地址和端口绑定
    //IP 地址和端口都保存在 sockaddr_in 结构体中
    struct sockaddr_in lst_addr;

    lst_addr.sin_family = AF_INET;
    // lst_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    lst_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    lst_addr.sin_port = htons(9997);

    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(sockfd, (struct sockaddr*) &lst_addr, len);

    if(ret < 0)
    {
        perror("bind error\n");
        return -1;
    }

    if(listen(sockfd, 5)<0)
    {
        perror("listen error\n");
        return -1;
    }

    while(1)
    {
        struct sockaddr_in cli_addr;
        int newfd = accept(sockfd, (struct sockaddr*)&cli_addr, &len);

        if(newfd<0)
        {
            perror("accept error\n");
            continue;
        }

        char buff[1024]={0};
        int ret = recv(newfd, buff, 1023, 0);

        if(ret > 0)
        {
            printf("req:%s\n",buff);
			//这里我们打印的是浏览器HTTP的请求头部
        }

        char *rsp = (char*)"<h1>hello world</h1>";
        memset(buff, 0x00, 1024);
        sprintf(buff, "%s\r\n%s%d\r\n%s\r\n\r\n%s%", "HTTP/1.1 200 OK","Content-Length: ", strlen(rsp), "Content-Type:text/html:charset=UTF-8", rsp);

		printf("%s", buff);
        send(newfd, buff, strlen(buff), 0);
        close(newfd);
    }
}