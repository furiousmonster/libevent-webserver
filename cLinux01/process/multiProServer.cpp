#include "../_freecplus.h"

// 程序退出时调用的函数
void FathEXIT(int sig);
void ChldEXIT(int sig);


CTcpServer TcpServer;   // 创建服务端对象。

int main(int argc,char *argv[])
{
  // 关闭全部的信号
  for (int ii=0;ii<100;ii++) signal(ii,SIG_IGN);
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  // 但请不要用 "kill -9 +进程号" 强行终止
  signal(SIGINT,FathEXIT); signal(SIGTERM,FathEXIT);

  if (TcpServer.InitServer(5858)==false) // 初始化TcpServer的通信端口。
  {
    printf("TcpServer.InitServer(5858) failed.\n"); return -1;
  }

  while (true)
  {
    if (TcpServer.Accept()==false)   // 等待客户端连接。
    {
      printf("TcpServer.Accept() failed.\n"); return -1;
    }

    if (fork()>0) { TcpServer.CloseClient(); continue; }
    signal(SIGINT,ChldEXIT); signal(SIGTERM,ChldEXIT);
    TcpServer.CloseListen();


    printf("客户端(%s)已连接。\n",TcpServer.GetIP());

    char strbuffer[1024];  // 存放数据的缓冲区。


    while (true)
    {

      memset(strbuffer,0,sizeof(strbuffer));
      if (TcpServer.Read(strbuffer,300)==false) break; // 接收客户端发过来的请求报文。
      printf("接收：%s\n",strbuffer);

      strcat(strbuffer,"ok");      // 在客户端的报文后加上"ok"。
      printf("发送：%s\n",strbuffer);
      if (TcpServer.Write(strbuffer)==false) break;     // 向客户端回应报文。
    }

    printf("客户端已断开。\n");    // 程序直接退出，析构函数会释放资源。
    ChldEXIT(0);
  }
}


// 父进程退出时调用的函数
void FathEXIT(int sig)

{
  if (sig > 0)
  {
    signal(sig,SIG_IGN); signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);
    printf("catching the signal(%d).\n",sig);
  }

  kill(0,15);
  printf("父进程退出。\n");


  // 编写善后代码（释放资源、提交或回滚事务）

  exit(0);
}



// 子进程退出时调用的函数

void ChldEXIT(int sig)
{
  if (sig > 0)
  {
    signal(sig,SIG_IGN); signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);
  }

  printf("子进程退出。\n");
  // 编写善后代码（释放资源、提交或回滚事务）

  exit(0);
}
