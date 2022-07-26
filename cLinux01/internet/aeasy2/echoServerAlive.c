#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define  BUF_SIZE 100
 
int main()
{
	//创建套接字
	int serv_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//将套接字和ip、端口绑定
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));//每个字节都用0填充
	serv_addr.sin_family=AF_INET;//使用ipv4地址
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(9000);
	bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
 
	//进入监听状态，等待用户发起请求
	listen(serv_sock,20);	
	
	//创建buffer client_addr
	char buffer[BUF_SIZE];

  struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size=sizeof(clnt_addr);

  while(1) {
    //接收客户端请求
    int clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

    read(clnt_sock,buffer,sizeof(buffer));
    printf("服务器端接收到的消息：%s\n",buffer);
    write(clnt_sock,buffer,sizeof(buffer));

    //关闭套接字
    close(clnt_sock);
    memset(buffer, 0, BUF_SIZE); //重置缓冲区
  }

	close(serv_sock);
	
	return 0;
}