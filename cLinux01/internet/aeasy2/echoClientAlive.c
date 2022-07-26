#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define  BUF_SIZE 100
 
int main()
{
	// 初始化连接服务器sockaddr结构
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(9000);

  // 缓冲区
  char buffersend[BUF_SIZE]={0};
  char bufferrecv[BUF_SIZE]={0};

  while(1) {
    //创建套接字并连接
    int sock=socket(AF_INET,SOCK_STREAM,0);
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    //向服务器发送数据
    printf("输入发送信息：");
    fgets(buffersend, 100, stdin);

    write(sock,buffersend,sizeof(buffersend));
    read(sock,bufferrecv,sizeof(bufferrecv)-1);

    //输出接收到的消息
    printf("接收到的消息：%s\n",bufferrecv);

    //关闭套接字
    memset(buffersend, 0, BUF_SIZE);
    memset(bufferrecv, 0, BUF_SIZE);
    close(sock);

  }

	return 0;
}