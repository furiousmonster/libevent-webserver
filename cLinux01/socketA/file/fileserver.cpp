/*
 *  程序名：demo48.cpp，此程序演示采用freecplus框架的CTcpServer类实现socket通信的服务端。
 *  作者：C语言技术网(www.freecplus.net) 日期：20190525
*/
#include "../_freecplus.h"
 
CTcpServer TcpServer;
void _fileserver();

char pathname[301];

int main(int argc,char *argv[])
{

  // 判断是否传入文件参数
  if (argc != 2)
  {
    printf("Using:./fileclient filepath\n");
    printf("Sample:./fileclient ./tmp/ \n");
    return -1;
  }

  strcpy(pathname, argv[1]);

  signal(SIGCHLD, SIG_IGN); //忽略子进程退出信号

  if (TcpServer.InitServer(5858) == false)
  {
    printf("服务端初始化失败，程序退出\n");
    return -1;
  }

  while(1)
  {
    if (TcpServer.Accept() == false) continue;

    if (fork() > 0) //父进程重新循环，accept
    {
      TcpServer.CloseClient();
      continue;
    }

    // 子进程负责与客户端通信，直到客户端断开连接
    TcpServer.CloseListen();
    printf("客户端（%s)已连接\n", TcpServer.GetIP());

    _fileserver();
    
    return 0;
  }
}

void _fileserver()
{
  char strbuffer[1024];  // 存放数据的缓冲区。

  memset(strbuffer,0,sizeof(strbuffer));
  //if (TcpServer.Read(strbuffer,300)==false) break; // 接收客户端发过来的请求报文。
  if (TcpServer.Read(strbuffer,10)==false) return; // 接收客户端发过来的请求报文。
  printf("接收：%s\n",strbuffer);


  // 接收文件内容
  char strfilename[301], strfullfilename[301];
  int isize = 0;

  memset(strfilename, 0, sizeof(strfilename));
  memset(strfullfilename, 0, sizeof(strfullfilename));

  GetXMLBuffer(strbuffer, "filename", strfilename);
  GetXMLBuffer(strbuffer, "size", &isize);

  sprintf(strfilename, "%s, %s", pathname, strfilename);

  CFile File;
  if (File.OpenForRename(strfilename, "w") == false)
  {
    strcpy(strbuffer, "<ret>-1</ret><message>failed</message>");
    if (TcpServer.Write(strbuffer, strlen(strbuffer)) <= 0) return;
  }

  // 文件读取代码1
  // int nLeft, nread, idx;
  // nLeft = isize;
  // idx = 0;
  // while (nLeft > 0)
  // {
  //   memset(strbuffer, 0, sizeof(strbuffer));
  //   if ( (nread = recv(TcpServer.m_connfd, strbuffer, nLeft, 0)) <= 0) return false;
  //   File.Fwrite(strbuffer, nread);

  //   idx += nread;
  //   nLeft -= nread;
  // }
  
  // 文件读取代码2
  int total_bytes = 0, onread;

  while (true)
  {
    memset(strbuffer, 0, sizeof(strbuffer));

    if ( (isize - total_bytes) > 1000) onread = 1000;
    else onread = isize - total_bytes;

    if (Readn(TcpServer.m_connfd, strbuffer, onread) == false) return;
    File.Fwrite(strbuffer, onread);

    total_bytes += onread;

    if ((int)total_bytes == isize) break;
  }
  

  File.CloseAndRename();

  strcat(strbuffer,"<ret>0</ret><message>ok</message>");      // 在客户端的报文后加上"ok"。
  if (TcpServer.Write(strbuffer)==false) return;     // 向客户端回应报文。
  printf("发送：%s\n",strbuffer);

  return;

}