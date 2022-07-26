#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// #include "../_freecplus.h"
 
class CTcpServer
{
public:
  int m_listenfd;   // 服务端用于监听的socket
  int m_clientfd;   // 客户端连上来的socket
 
  CTcpServer();
 
  bool InitServer(int port);  // 初始化服务端
 
  bool Accept();  // 等待客户端的连接
 
  // 向对端发送报文
  int  Send(const void *buf,const int buflen);
  // 接收对端的报文
  int  Recv(void *buf,const int buflen);
 
  // void CloseClient();    // 关闭客户端的socket，多线程服务端不需要这个函数。
  // void CloseListen();    // 关闭用于监听的socket，多线程服务端不需要这个函数。
 
 ~CTcpServer();
};
 
CTcpServer TcpServer;
 
// SIGINT和SIGTERM的处理函数
void EXIT(int sig)
{
  printf("程序退出，信号值=%d\n",sig);
 
  close(TcpServer.m_listenfd);  // 手动关闭m_listenfd，释放资源
 
  exit(0);
}
 
// 与客户端通信线程的主函数
void *pth_main(void *arg);
 
int main()
{
  // 忽略全部的信号
  for (int ii=0;ii<50;ii++) signal(ii,SIG_IGN);
 
  // 设置SIGINT和SIGTERM的处理函数
  signal(SIGINT,EXIT); signal(SIGTERM,EXIT);
 
  if (TcpServer.InitServer(5051)==false)
  { printf("服务端初始化失败，程序退出。\n"); return -1; }
 
  while (1)
  {
    if (TcpServer.Accept() == false) continue;
 
    pthread_t pthid;   // 创建一线程，与新连接上来的客户端通信
    if (pthread_create(&pthid, NULL, pth_main, (void*)((long)TcpServer.m_clientfd)) != 0)
    { 
      printf("创建线程失败，程序退出。n");
      return -1; 
    }
 
    printf("与客户端通信的线程已创建。\n");
  }
}
 
CTcpServer::CTcpServer()
{
  // 构造函数初始化socket
  m_listenfd=m_clientfd=0;
}
 
CTcpServer::~CTcpServer()
{
  if (m_listenfd!=0) close(m_listenfd);  // 析构函数关闭socket
  if (m_clientfd!=0) close(m_clientfd);  // 析构函数关闭socket
}
 
// 初始化服务端的socket，port为通信端口
bool CTcpServer::InitServer(int port)
{
  if (m_listenfd!=0) { close(m_listenfd); m_listenfd=0; }
 
  m_listenfd = socket(AF_INET,SOCK_STREAM,0);  // 创建服务端的socket
 
  // 把服务端用于通信的地址和端口绑定到socket上
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本主机的任意ip地址
  servaddr.sin_port = htons(port);  // 绑定通信端口
  if (bind(m_listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
  { close(m_listenfd); m_listenfd=0; return false; }
 
  // 把socket设置为监听模式
  if (listen(m_listenfd,5) != 0 ) { close(m_listenfd); m_listenfd=0; return false; }
 
  return true;
}
 
bool CTcpServer::Accept()
{
  if ( (m_clientfd=accept(m_listenfd,0,0)) <= 0) return false;
 
  return true;
}
 
int CTcpServer::Send(const void *buf,const int buflen)
{
  return send(m_clientfd,buf,buflen,0);
}
 
int CTcpServer::Recv(void *buf,const int buflen)
{
  return recv(m_clientfd,buf,buflen,0);
}
 
// 与客户端通信线程的主函数
void *pth_main(void *arg)
{
  int clientfd=(long) arg; // arg参数为新客户端的socket。
 
  // 与客户端通信，接收客户端发过来的报文后，回复ok。
  char strbuffer[1024];
 
  while (1)
  {
    memset(strbuffer,0,sizeof(strbuffer));
    if (recv(clientfd,strbuffer,sizeof(strbuffer),0)<=0) break;
    printf("接收：%s\n",strbuffer);
 
    strcpy(strbuffer,"ok");
    if (send(clientfd,strbuffer,strlen(strbuffer),0)<=0) break;
    printf("发送：%s\n",strbuffer);
  }
 
  printf("客户端已断开连接。\n");
 
  close(clientfd);  // 关闭客户端的连接。
 
  pthread_exit(0);
}